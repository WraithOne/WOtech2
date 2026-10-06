////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_RenderPipeline.cpp
///
///			Description:
///			HDR target, SSAO, PCF shadow of the feature mesh, and IBL
///			(irradiance, prefiltered specular, BRDF LUT) sampled by PBR.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_RenderPipeline.hpp"

#include "cgltf.h"

#include <d3dcompiler.h>

#include <fstream>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

namespace WOtech
{
	namespace
	{
		char const* kFullscreenVs = R"(
struct VSOut { float4 pos : SV_Position; float2 uv : TEXCOORD0; };
VSOut main(uint id : SV_VertexID) {
	float2 uv = float2((id << 1) & 2, id & 2);
	VSOut o;
	o.pos = float4(uv * float2(2, -2) + float2(-1, 1), 0, 1);
	o.uv = uv;
	return o;
})";

		char const* kMeshVs = R"(
cbuffer Frame : register(b0) {
	row_major float4x4 World;
	row_major float4x4 ViewProj;
	row_major float4x4 LightViewProj;
	float4 CameraPos;
	float4 LightDir;
	float4 Params;
	float4 Misc;
	float4 Normal;
};
struct VSIn { float3 pos : POSITION; };
struct VSOut { float4 pos : SV_Position; float3 world : TEXCOORD0; float3 n : TEXCOORD1; };
VSOut main(VSIn vin) {
	VSOut o;
	float4 world = mul(float4(vin.pos, 1), World);
	o.world = world.xyz;
	o.pos = mul(world, ViewProj);
	o.n = normalize(mul(Normal.xyz, (float3x3)World));
	return o;
})";

		char const* kShared = R"(
cbuffer Frame : register(b0) {
	row_major float4x4 World;
	row_major float4x4 ViewProj;
	row_major float4x4 LightViewProj;
	float4 CameraPos;
	float4 LightDir;
	float4 Params;
	float4 Misc;
	float4 Normal;
};
float3 FaceDir(float2 uv, int face) {
	float2 d = uv * 2.0 - 1.0;
	if (face == 0) return normalize(float3(1, -d.y, -d.x));
	if (face == 1) return normalize(float3(-1, -d.y, d.x));
	if (face == 2) return normalize(float3(d.x, 1, d.y));
	if (face == 3) return normalize(float3(d.x, -1, -d.y));
	if (face == 4) return normalize(float3(d.x, -d.y, 1));
	return normalize(float3(-d.x, -d.y, -1));
}
float2 EquirectUv(float3 d) {
	float phi = atan2(d.z, d.x);
	float theta = acos(clamp(d.y, -1.0, 1.0));
	return float2(phi / 6.2831853 + 0.5, theta / 3.14159265);
}
float RadicalInverse(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}
float2 Hammersley(uint i, uint n) { return float2(float(i) / float(n), RadicalInverse(i)); }
float3 TangentToWorld(float3 n, float3 h) {
	float3 up = abs(n.y) < 0.999 ? float3(0, 1, 0) : float3(1, 0, 0);
	float3 t = normalize(cross(up, n));
	float3 b = cross(n, t);
	return normalize(t * h.x + b * h.y + n * h.z);
}
float3 ImportanceGGX(float2 xi, float roughness, float3 n) {
	float a = roughness * roughness;
	float phi = 6.2831853 * xi.x;
	float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
	float sinTheta = sqrt(max(0.0, 1.0 - cosTheta * cosTheta));
	float3 h = float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
	return TangentToWorld(n, h);
}
float GeometrySchlick(float nDot, float k) { return nDot / (nDot * (1.0 - k) + k); }
)";

		char const* kEquirectPs = R"(
Texture2D equirect : register(t0);
SamplerState sam : register(s0);
)" 
		R"(
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float3 dir = FaceDir(uv, (int)Misc.x);
	return equirect.SampleLevel(sam, EquirectUv(dir), 0);
})";

		char const* kIrradiancePs = R"(
TextureCube envMap : register(t1);
SamplerState sam : register(s0);
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float3 n = FaceDir(uv, (int)Misc.x);
	float3 acc = 0;
	float weight = 0;
	const uint count = 32;
	[loop] for (uint i = 0; i < count; ++i) {
		float2 xi = Hammersley(i, count);
		float phi = 6.2831853 * xi.x;
		float cosTheta = sqrt(1.0 - xi.y);
		float sinTheta = sqrt(xi.y);
		float3 h = float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
		float3 l = TangentToWorld(n, h);
		float nDotL = saturate(dot(n, l));
		acc += envMap.SampleLevel(sam, l, 0).rgb * nDotL;
		weight += nDotL;
	}
	return float4(acc / max(weight, 0.0001), 1);
})";

		char const* kPrefilterPs = R"(
TextureCube envMap : register(t1);
SamplerState sam : register(s0);
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float3 n = FaceDir(uv, (int)Misc.x);
	float roughness = saturate(Misc.y);
	float3 acc = 0;
	float weight = 0;
	const uint count = 32;
	[loop] for (uint i = 0; i < count; ++i) {
		float2 xi = Hammersley(i, count);
		float3 h = ImportanceGGX(xi, max(roughness, 0.04), n);
		float3 l = normalize(2.0 * dot(n, h) * h - n);
		float nDotL = saturate(dot(n, l));
		if (nDotL > 0) {
			acc += envMap.SampleLevel(sam, l, 0).rgb * nDotL;
			weight += nDotL;
		}
	}
	return float4(acc / max(weight, 0.0001), 1);
})";

		char const* kBrdfPs = R"(
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float nDotV = max(uv.x, 0.001);
	float roughness = max(uv.y, 0.04);
	float3 v = float3(sqrt(1.0 - nDotV * nDotV), 0, nDotV);
	float3 n = float3(0, 0, 1);
	float a = 0;
	float b = 0;
	const uint count = 32;
	[loop] for (uint i = 0; i < count; ++i) {
		float3 h = ImportanceGGX(Hammersley(i, count), roughness, n);
		float3 l = normalize(2.0 * dot(v, h) * h - v);
		float nDotL = saturate(l.z);
		float nDotH = saturate(h.z);
		float vDotH = saturate(dot(v, h));
		if (nDotL > 0) {
			float k = (roughness + 1);
			k = (k * k) / 8.0;
			float g = GeometrySchlick(nDotV, k) * GeometrySchlick(nDotL, k);
			float gVis = (g * vDotH) / max(nDotH * nDotV, 0.0001);
			float fc = pow(1.0 - vDotH, 5.0);
			a += (1.0 - fc) * gVis;
			b += fc * gVis;
		}
	}
	return float4(a / count, b / count, 0, 1);
})";

		char const* kDepthPs = R"(
struct VSOut { float4 pos : SV_Position; float3 world : TEXCOORD0; float3 n : TEXCOORD1; };
float4 main(VSOut pin) : SV_Target {
	float4 clip = mul(float4(pin.world, 1), ViewProj);
	return clip.z / clip.w;
})";

		char const* kSsao = R"(
Texture2D depthTex : register(t0);
SamplerState sam : register(s0);
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float center = depthTex.Sample(sam, uv).r;
	float occ = 0;
	[unroll] for (int i = 0; i < 8; ++i) {
		float2 xi = Hammersley((uint)i, 8u);
		float2 s = uv + (xi * 2.0 - 1.0) * Params.x;
		float d = depthTex.Sample(sam, s).r;
		float range = abs(center - d) < Params.y ? 1.0 : 0.0;
		occ += (d > center + 0.0005) ? range : 0;
	}
	return float4(1.0 - occ / 8.0, center, 0, 1);
})";

		char const* kPbrPs = R"(
Texture2D ssaoTex : register(t0);
TextureCube irradiance : register(t1);
TextureCube prefilter : register(t2);
Texture2D brdfLut : register(t3);
Texture2D shadowMap : register(t4);
SamplerState sam : register(s0);
struct VSOut { float4 pos : SV_Position; float3 world : TEXCOORD0; float3 n : TEXCOORD1; };
float ShadowPcf(float3 worldPos) {
	float4 clip = mul(float4(worldPos, 1), LightViewProj);
	float3 proj = clip.xyz / clip.w;
	float2 uv = proj.xy * float2(0.5, -0.5) + 0.5;
	float refDepth = proj.z - Params.w;
	float texel = Params.z;
	float sum = 0;
	[unroll] for (int y = -1; y <= 1; ++y) {
		[unroll] for (int x = -1; x <= 1; ++x) {
			float mapDepth = shadowMap.Sample(sam, uv + float2(x, y) * texel).r;
			sum += refDepth <= mapDepth ? 1.0 : 0.15;
		}
	}
	return sum / 9.0;
}
float4 main(VSOut pin) : SV_Target {
	float3 n = normalize(pin.n);
	float3 v = normalize(CameraPos.xyz - pin.world);
	float3 r = reflect(-v, n);
	float nDotV = saturate(dot(n, v));
	float metallic = Params.x;
	float roughness = Params.y;
	float3 albedo = float3(0.8, 0.2, 0.1);
	float3 irradianceColor = irradiance.Sample(sam, n).rgb;
	float3 specColor = prefilter.SampleLevel(sam, r, roughness * Misc.y).rgb;
	float2 brdf = brdfLut.Sample(sam, float2(nDotV, roughness)).rg;
	float3 f0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
	float3 diffuse = irradianceColor * albedo * (1.0 - metallic);
	float3 specular = specColor * (f0 * brdf.x + brdf.y);
	float shadow = ShadowPcf(pin.world);
	float2 screenUv = pin.pos.xy / float2(Misc.z, Misc.w);
	float ao = ssaoTex.Sample(sam, screenUv).r;
	float nDotL = saturate(dot(n, LightDir.xyz));
	float3 direct = albedo * nDotL * shadow;
	float3 color = (diffuse + specular + direct) * ao;
	return float4(color, 1);
})";

		char const* kTonemap = R"(
Texture2D hdr : register(t0);
SamplerState sam : register(s0);
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float3 c = hdr.Sample(sam, uv).rgb;
	c = c / (1.0 + c);
	c = pow(max(c, 0), 1.0 / 2.2);
	return float4(c, 1);
})";

		struct FrameCB
		{
			DirectX::XMFLOAT4X4 World;
			DirectX::XMFLOAT4X4 ViewProj;
			DirectX::XMFLOAT4X4 LightViewProj;
			DirectX::XMFLOAT4 CameraPos;
			DirectX::XMFLOAT4 LightDir;
			DirectX::XMFLOAT4 Params;
			DirectX::XMFLOAT4 Misc;
			DirectX::XMFLOAT4 Normal;
		};

		bool Compile(_In_z_ const char* source, _In_z_ const char* profile, _Out_ ID3DBlob** blob, _Out_opt_ std::string* error)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> errors;
			HRESULT const hr = D3DCompile(source, strlen(source), nullptr, nullptr, nullptr, "main", profile, D3DCOMPILE_OPTIMIZATION_LEVEL1, 0, blob, errors.GetAddressOf());
			if (FAILED(hr))
			{
				if (error != nullptr)
				{
					*error = errors ? static_cast<char const*>(errors->GetBufferPointer()) : "shader compile failed";
				}
				return false;
			}
			return true;
		}

		std::string JoinShader(_In_z_ const char* body)
		{
			return std::string(kShared) + body;
		}

		bool FileExists(_In_z_ const char* path)
		{
			DWORD const attr = GetFileAttributesA(path);
			return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
		}

		bool FindAsset(_In_z_ const char* relative, _Out_ std::string* full)
		{
			if (FileExists(relative))
			{
				*full = relative;
				return true;
			}
			char bases[2][MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, bases[0]);
			GetModuleFileNameA(nullptr, bases[1], MAX_PATH);
			char* slash = strrchr(bases[1], '\\');
			if (slash != nullptr)
			{
				*slash = '\0';
			}
			for (int baseIndex = 0; baseIndex < 2; ++baseIndex)
			{
				std::string dir = bases[baseIndex];
				for (int up = 0; up < 6; ++up)
				{
					std::string candidate = dir + "\\" + relative;
					if (FileExists(candidate.c_str()))
					{
						*full = candidate;
						return true;
					}
					size_t const pos = dir.find_last_of("\\/");
					if (pos == std::string::npos)
					{
						break;
					}
					dir.resize(pos);
				}
			}
			return false;
		}

		float RgbeToFloat(_In_ UINT8 channel, _In_ UINT8 exponent)
		{
			if (exponent == 0)
			{
				return 0.0f;
			}
			return ldexpf(static_cast<float>(channel), static_cast<int>(exponent) - 128 - 8);
		}

		bool LoadHdr(_In_z_ const char* path, _Out_ std::vector<float>* rgb, _Out_ UINT* width, _Out_ UINT* height, _Out_opt_ std::string* error)
		{
			std::ifstream file(path, std::ios::binary);
			if (!file)
			{
				if (error != nullptr) *error = "hdri missing";
				return false;
			}
			std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			size_t cursor = 0;
			auto nextLine = [&]() -> std::string
			{
				size_t end = cursor;
				while (end < bytes.size() && bytes[end] != '\n')
				{
					++end;
				}
				std::string line(bytes.begin() + static_cast<std::ptrdiff_t>(cursor), bytes.begin() + static_cast<std::ptrdiff_t>(end));
				cursor = end < bytes.size() ? end + 1 : end;
				if (!line.empty() && line.back() == '\r')
				{
					line.pop_back();
				}
				return line;
			};
			bool sawFormat = false;
			for (int i = 0; i < 16 && cursor < bytes.size(); ++i)
			{
				std::string line = nextLine();
				if (line.empty())
				{
					break;
				}
				if (line.find("FORMAT=") != std::string::npos)
				{
					sawFormat = true;
				}
			}
			if (!sawFormat)
			{
				if (error != nullptr) *error = "hdri format missing";
				return false;
			}
			std::string resolution = nextLine();
			int y = 0;
			int x = 0;
			if (sscanf_s(resolution.c_str(), "-Y %d +X %d", &y, &x) != 2 || x <= 0 || y <= 0)
			{
				if (error != nullptr) *error = "hdri resolution missing";
				return false;
			}
			*width = static_cast<UINT>(x);
			*height = static_cast<UINT>(y);
			rgb->assign(static_cast<size_t>(*width) * *height * 4u, 0.0f);
			UINT8 const* pixels = reinterpret_cast<UINT8 const*>(bytes.data() + cursor);
			size_t const remain = bytes.size() - cursor;
			if (remain < static_cast<size_t>(*width) * *height * 4u)
			{
				if (error != nullptr) *error = "hdri truncated";
				return false;
			}
			for (UINT row = 0; row < *height; ++row)
			{
				UINT8 const* scan = pixels + static_cast<size_t>(row) * *width * 4u;
				for (UINT col = 0; col < *width; ++col)
				{
					size_t const dst = (static_cast<size_t>(row) * *width + col) * 4u;
					(*rgb)[dst + 0] = RgbeToFloat(scan[col * 4u + 0], scan[col * 4u + 3]);
					(*rgb)[dst + 1] = RgbeToFloat(scan[col * 4u + 1], scan[col * 4u + 3]);
					(*rgb)[dst + 2] = RgbeToFloat(scan[col * 4u + 2], scan[col * 4u + 3]);
					(*rgb)[dst + 3] = 1.0f;
				}
			}
			return true;
		}

		bool LoadFeatureMesh(_Out_ std::vector<DirectX::XMFLOAT3>* vertices, _Out_opt_ std::string* error)
		{
			std::string path;
			if (!FindAsset("Assets\\Models\\Triangle.gltf", &path))
			{
				if (error != nullptr) *error = "feature mesh missing";
				return false;
			}
			cgltf_options options = {};
			cgltf_data* data = nullptr;
			if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success || data == nullptr)
			{
				if (error != nullptr) *error = "feature mesh parse failed";
				return false;
			}
			cgltf_load_buffers(&options, data, path.c_str());
			bool loaded = false;
			if (data->meshes_count > 0 && data->meshes[0].primitives_count > 0)
			{
				cgltf_primitive const& primitive = data->meshes[0].primitives[0];
				for (cgltf_size i = 0; i < primitive.attributes_count; ++i)
				{
					if (primitive.attributes[i].type != cgltf_attribute_type_position || primitive.attributes[i].data == nullptr)
					{
						continue;
					}
					cgltf_accessor* accessor = primitive.attributes[i].data;
					vertices->resize(accessor->count);
					for (cgltf_size v = 0; v < accessor->count; ++v)
					{
						float xyz[3] = {};
						if (!cgltf_accessor_read_float(accessor, v, xyz, 3))
						{
							vertices->clear();
							break;
						}
						(*vertices)[v] = DirectX::XMFLOAT3(xyz[0], xyz[1], xyz[2]);
					}
					loaded = vertices->size() >= 3;
					break;
				}
			}
			cgltf_free(data);
			if (!loaded && error != nullptr)
			{
				*error = "feature mesh has no positions";
			}
			return loaded;
		}

		bool CreateShader(_In_ ID3D11Device* device, _In_z_ const char* source, _In_ bool pixel, _Out_ Microsoft::WRL::ComPtr<ID3D11DeviceChild>* shader, _Out_opt_ ID3DBlob** blob, _Out_opt_ std::string* error)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> compiled;
			if (!Compile(source, pixel ? "ps_5_0" : "vs_5_0", compiled.GetAddressOf(), error))
			{
				return false;
			}
			HRESULT hr = E_FAIL;
			if (pixel)
			{
				Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
				hr = device->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ps.GetAddressOf());
				*shader = ps;
			}
			else
			{
				Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
				hr = device->CreateVertexShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, vs.GetAddressOf());
				*shader = vs;
			}
			if (FAILED(hr))
			{
				if (error != nullptr) *error = "shader create failed";
				return false;
			}
			if (blob != nullptr)
			{
				*blob = compiled.Detach();
			}
			return true;
		}

		void BindFullscreen(_In_ ID3D11DeviceContext* context, _In_ ID3D11VertexShader* vs, _In_ ID3D11PixelShader* ps)
		{
			context->IASetInputLayout(nullptr);
			context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->VSSetShader(vs, nullptr, 0);
			context->PSSetShader(ps, nullptr, 0);
		}

		void UnbindSrvs(_In_ ID3D11DeviceContext* context)
		{
			ID3D11ShaderResourceView* nulls[5] = {};
			context->PSSetShaderResources(0, 5, nulls);
		}
	}

	bool RenderPipeline::RenderFeatureFrame(_Out_opt_ std::string* error)
	{
		std::string hdrPath;
		std::vector<float> hdrPixels;
		UINT hdrWidth = 0;
		UINT hdrHeight = 0;
		if (!FindAsset("Assets\\HDRI\\sample_studio.hdr", &hdrPath) || !LoadHdr(hdrPath.c_str(), &hdrPixels, &hdrWidth, &hdrHeight, error))
		{
			if (error != nullptr && error->empty()) *error = "hdri missing";
			return false;
		}
		std::vector<DirectX::XMFLOAT3> vertices;
		if (!LoadFeatureMesh(&vertices, error))
		{
			return false;
		}

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &level, 1, D3D11_SDK_VERSION, device.GetAddressOf(), nullptr, context.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "no device";
			return false;
		}

		std::string equirectSource = JoinShader(kEquirectPs);
		std::string irradianceSource = JoinShader(kIrradiancePs);
		std::string prefilterSource = JoinShader(kPrefilterPs);
		std::string brdfSource = JoinShader(kBrdfPs);
		std::string depthSource = JoinShader(kDepthPs);
		std::string ssaoSource = JoinShader(kSsao);
		std::string pbrSource = JoinShader(kPbrPs);
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> fullscreenVsChild;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> meshVsChild;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> equirectPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> irradiancePs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> prefilterPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> brdfPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> depthPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> ssaoPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> pbrPs;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> tonePs;
		Microsoft::WRL::ComPtr<ID3DBlob> meshBlob;
		if (!CreateShader(device.Get(), kFullscreenVs, false, &fullscreenVsChild, nullptr, error)) return false;
		if (!CreateShader(device.Get(), kMeshVs, false, &meshVsChild, meshBlob.GetAddressOf(), error)) return false;
		if (!CreateShader(device.Get(), equirectSource.c_str(), true, &equirectPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), irradianceSource.c_str(), true, &irradiancePs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), prefilterSource.c_str(), true, &prefilterPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), brdfSource.c_str(), true, &brdfPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), depthSource.c_str(), true, &depthPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), ssaoSource.c_str(), true, &ssaoPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), pbrSource.c_str(), true, &pbrPs, nullptr, error)) return false;
		if (!CreateShader(device.Get(), kTonemap, true, &tonePs, nullptr, error)) return false;

		D3D11_INPUT_ELEMENT_DESC layoutDesc = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
		hr = device->CreateInputLayout(&layoutDesc, 1, meshBlob->GetBufferPointer(), meshBlob->GetBufferSize(), layout.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "input layout failed";
			return false;
		}
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(DirectX::XMFLOAT3));
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vbData = {};
		vbData.pSysMem = vertices.data();
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		hr = device->CreateBuffer(&vbDesc, &vbData, vertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "mesh buffer failed";
			return false;
		}

		D3D11_TEXTURE2D_DESC equirectDesc = {};
		equirectDesc.Width = hdrWidth;
		equirectDesc.Height = hdrHeight;
		equirectDesc.MipLevels = 1;
		equirectDesc.ArraySize = 1;
		equirectDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		equirectDesc.SampleDesc.Count = 1;
		equirectDesc.Usage = D3D11_USAGE_IMMUTABLE;
		equirectDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA equirectData = {};
		equirectData.pSysMem = hdrPixels.data();
		equirectData.SysMemPitch = hdrWidth * sizeof(float) * 4u;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> equirectTex;
		hr = device->CreateTexture2D(&equirectDesc, &equirectData, equirectTex.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "hdri upload failed";
			return false;
		}
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> equirectSrv;
		device->CreateShaderResourceView(equirectTex.Get(), nullptr, equirectSrv.GetAddressOf());

		auto createCube = [&](UINT size, UINT mips, DXGI_FORMAT format, Microsoft::WRL::ComPtr<ID3D11Texture2D>* tex, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* srv) -> bool
		{
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = size;
			desc.Height = size;
			desc.MipLevels = mips;
			desc.ArraySize = 6;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
			if (FAILED(device->CreateTexture2D(&desc, nullptr, tex->GetAddressOf())))
			{
				return false;
			}
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = mips;
			return SUCCEEDED(device->CreateShaderResourceView(tex->Get(), &srvDesc, srv->GetAddressOf()));
		};
		auto faceRtv = [&](ID3D11Texture2D* tex, UINT face, UINT mip, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>* rtv) -> bool
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.MipSlice = mip;
			desc.Texture2DArray.FirstArraySlice = face;
			desc.Texture2DArray.ArraySize = 1;
			return SUCCEEDED(device->CreateRenderTargetView(tex, &desc, rtv->GetAddressOf()));
		};

		const UINT envSize = 16;
		const UINT irrSize = 8;
		const UINT specMips = 4;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> envTex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> irrTex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> specTex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> envSrv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> irrSrv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specSrv;
		if (!createCube(envSize, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, &envTex, &envSrv)
			|| !createCube(irrSize, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, &irrTex, &irrSrv)
			|| !createCube(envSize, specMips, DXGI_FORMAT_R16G16B16A16_FLOAT, &specTex, &specSrv))
		{
			if (error != nullptr) *error = "ibl cubemap failed";
			return false;
		}

		D3D11_TEXTURE2D_DESC lutDesc = {};
		lutDesc.Width = 64;
		lutDesc.Height = 64;
		lutDesc.MipLevels = 1;
		lutDesc.ArraySize = 1;
		lutDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		lutDesc.SampleDesc.Count = 1;
		lutDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> brdfTex;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> brdfRtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brdfSrv;
		if (FAILED(device->CreateTexture2D(&lutDesc, nullptr, brdfTex.GetAddressOf()))
			|| FAILED(device->CreateRenderTargetView(brdfTex.Get(), nullptr, brdfRtv.GetAddressOf()))
			|| FAILED(device->CreateShaderResourceView(brdfTex.Get(), nullptr, brdfSrv.GetAddressOf())))
		{
			if (error != nullptr) *error = "brdf lut failed";
			return false;
		}

		const UINT width = 128;
		const UINT height = 128;
		D3D11_TEXTURE2D_DESC targetDesc = {};
		targetDesc.Width = width;
		targetDesc.Height = height;
		targetDesc.MipLevels = 1;
		targetDesc.ArraySize = 1;
		targetDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		targetDesc.SampleDesc.Count = 1;
		targetDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> hdr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> hdrRtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> hdrSrv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthColor;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> depthRtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthSrv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> ssaoTex;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> ssaoRtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ssaoSrv;
		device->CreateTexture2D(&targetDesc, nullptr, hdr.GetAddressOf());
		device->CreateRenderTargetView(hdr.Get(), nullptr, hdrRtv.GetAddressOf());
		device->CreateShaderResourceView(hdr.Get(), nullptr, hdrSrv.GetAddressOf());
		targetDesc.Format = DXGI_FORMAT_R32_FLOAT;
		device->CreateTexture2D(&targetDesc, nullptr, depthColor.GetAddressOf());
		device->CreateRenderTargetView(depthColor.Get(), nullptr, depthRtv.GetAddressOf());
		device->CreateShaderResourceView(depthColor.Get(), nullptr, depthSrv.GetAddressOf());
		targetDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		device->CreateTexture2D(&targetDesc, nullptr, ssaoTex.GetAddressOf());
		device->CreateRenderTargetView(ssaoTex.Get(), nullptr, ssaoRtv.GetAddressOf());
		device->CreateShaderResourceView(ssaoTex.Get(), nullptr, ssaoSrv.GetAddressOf());

		D3D11_TEXTURE2D_DESC shadowDesc = {};
		shadowDesc.Width = 256;
		shadowDesc.Height = 256;
		shadowDesc.MipLevels = 1;
		shadowDesc.ArraySize = 1;
		shadowDesc.Format = DXGI_FORMAT_R32_FLOAT;
		shadowDesc.SampleDesc.Count = 1;
		shadowDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> shadow;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> shadowRtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadowSrv;
		device->CreateTexture2D(&shadowDesc, nullptr, shadow.GetAddressOf());
		device->CreateRenderTargetView(shadow.Get(), nullptr, shadowRtv.GetAddressOf());
		device->CreateShaderResourceView(shadow.Get(), nullptr, shadowSrv.GetAddressOf());

		targetDesc.Width = width;
		targetDesc.Height = height;
		targetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> ldr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> ldrRtv;
		device->CreateTexture2D(&targetDesc, nullptr, ldr.GetAddressOf());
		device->CreateRenderTargetView(ldr.Get(), nullptr, ldrRtv.GetAddressOf());

		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());

		using namespace DirectX;
		XMVECTOR const v0 = XMLoadFloat3(&vertices[0]);
		XMVECTOR const v1 = XMLoadFloat3(&vertices[1]);
		XMVECTOR const v2 = XMLoadFloat3(&vertices[2]);
		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0));
		if (XMVectorGetX(XMVector3LengthSq(normal)) < 0.0001f)
		{
			normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		}
		XMMATRIX const world = XMMatrixRotationX(-1.1f) * XMMatrixTranslation(0.0f, 0.4f, 0.0f);
		XMVECTOR const worldNormal = XMVector3Normalize(XMVector3TransformNormal(normal, world));
		XMVECTOR const lightDir = XMVector3Normalize(XMVectorSet(0.35f, 0.85f, 0.25f, 0.0f));
		XMVECTOR const eye = XMVectorSet(0.0f, 1.2f, -3.5f, 1.0f);
		XMMATRIX const view = XMMatrixLookAtLH(eye, XMVectorSet(0.0f, 0.3f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMMATRIX const proj = XMMatrixPerspectiveFovLH(1.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 30.0f);
		XMVECTOR const lightPos = lightDir * 8.0f;
		XMMATRIX const lightView = XMMatrixLookAtLH(lightPos, XMVectorZero(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMMATRIX const lightProj = XMMatrixOrthographicLH(4.0f, 4.0f, 0.1f, 20.0f);

		FrameCB cb = {};
		XMStoreFloat4x4(&cb.World, world);
		XMStoreFloat4x4(&cb.ViewProj, view * proj);
		XMStoreFloat4x4(&cb.LightViewProj, lightView * lightProj);
		XMStoreFloat4(&cb.CameraPos, eye);
		XMStoreFloat4(&cb.LightDir, lightDir);
		XMStoreFloat4(&cb.Normal, worldNormal);
		cb.Params = XMFLOAT4(0.1f, 0.4f, 1.0f / 256.0f, 0.005f);
		cb.Misc = XMFLOAT4(0.0f, static_cast<float>(specMips - 1), static_cast<float>(width), static_cast<float>(height));
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(FrameCB);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Microsoft::WRL::ComPtr<ID3D11Buffer> cbBuffer;
		if (FAILED(device->CreateBuffer(&cbDesc, nullptr, cbBuffer.GetAddressOf())))
		{
			if (error != nullptr) *error = "constant buffer failed";
			return false;
		}
		auto uploadCb = [&]()
		{
			D3D11_MAPPED_SUBRESOURCE mapped = {};
			if (SUCCEEDED(context->Map(cbBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
			{
				memcpy(mapped.pData, &cb, sizeof(cb));
				context->Unmap(cbBuffer.Get(), 0);
			}
		};
		auto setViewport = [&](float size)
		{
			D3D11_VIEWPORT viewport = { 0, 0, size, size, 0, 1 };
			context->RSSetViewports(1, &viewport);
		};
		ID3D11VertexShader* fullscreenVs = static_cast<ID3D11VertexShader*>(fullscreenVsChild.Get());
		ID3D11VertexShader* meshVs = static_cast<ID3D11VertexShader*>(meshVsChild.Get());
		context->PSSetSamplers(0, 1, sampler.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, cbBuffer.GetAddressOf());
		context->VSSetConstantBuffers(0, 1, cbBuffer.GetAddressOf());

		for (UINT face = 0; face < 6; ++face)
		{
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
			if (!faceRtv(envTex.Get(), face, 0, &rtv))
			{
				if (error != nullptr) *error = "env face failed";
				return false;
			}
			cb.Misc.x = static_cast<float>(face);
			uploadCb();
			setViewport(static_cast<float>(envSize));
			context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
			UnbindSrvs(context.Get());
			context->PSSetShaderResources(0, 1, equirectSrv.GetAddressOf());
			BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(equirectPs.Get()));
			context->Draw(3, 0);
		}
		for (UINT face = 0; face < 6; ++face)
		{
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
			if (!faceRtv(irrTex.Get(), face, 0, &rtv))
			{
				if (error != nullptr) *error = "irradiance face failed";
				return false;
			}
			cb.Misc.x = static_cast<float>(face);
			uploadCb();
			setViewport(static_cast<float>(irrSize));
			UnbindSrvs(context.Get());
			context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
			context->PSSetShaderResources(1, 1, envSrv.GetAddressOf());
			BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(irradiancePs.Get()));
			context->Draw(3, 0);
		}
		for (UINT mip = 0; mip < specMips; ++mip)
		{
			UINT const mipSize = envSize >> mip;
			cb.Misc.y = specMips == 1 ? 0.0f : static_cast<float>(mip) / static_cast<float>(specMips - 1);
			for (UINT face = 0; face < 6; ++face)
			{
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
				if (!faceRtv(specTex.Get(), face, mip, &rtv))
				{
					if (error != nullptr) *error = "prefilter face failed";
					return false;
				}
				cb.Misc.x = static_cast<float>(face);
				uploadCb();
				setViewport(static_cast<float>(mipSize));
				UnbindSrvs(context.Get());
				context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
				context->PSSetShaderResources(1, 1, envSrv.GetAddressOf());
				BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(prefilterPs.Get()));
				context->Draw(3, 0);
			}
		}
		cb.Misc.y = static_cast<float>(specMips - 1);
		uploadCb();
		setViewport(64.0f);
		UnbindSrvs(context.Get());
		context->OMSetRenderTargets(1, brdfRtv.GetAddressOf(), nullptr);
		BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(brdfPs.Get()));
		context->Draw(3, 0);

		auto drawMesh = [&](ID3D11RenderTargetView* target, ID3D11PixelShader* ps, float viewWidth, float viewHeight)
		{
			D3D11_VIEWPORT viewport = { 0, 0, viewWidth, viewHeight, 0, 1 };
			context->RSSetViewports(1, &viewport);
			UnbindSrvs(context.Get());
			context->OMSetRenderTargets(1, &target, nullptr);
			UINT stride = sizeof(XMFLOAT3);
			UINT offset = 0;
			context->IASetInputLayout(layout.Get());
			context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->VSSetShader(meshVs, nullptr, 0);
			context->PSSetShader(ps, nullptr, 0);
			context->Draw(static_cast<UINT>(vertices.size()), 0);
		};

		FLOAT const farDepth[4] = { 1, 1, 1, 1 };
		context->ClearRenderTargetView(shadowRtv.Get(), farDepth);
		XMFLOAT4X4 cameraViewProj = cb.ViewProj;
		cb.ViewProj = cb.LightViewProj;
		uploadCb();
		drawMesh(shadowRtv.Get(), static_cast<ID3D11PixelShader*>(depthPs.Get()), 256.0f, 256.0f);
		cb.ViewProj = cameraViewProj;
		uploadCb();
		context->ClearRenderTargetView(depthRtv.Get(), farDepth);
		drawMesh(depthRtv.Get(), static_cast<ID3D11PixelShader*>(depthPs.Get()), static_cast<float>(width), static_cast<float>(height));

		D3D11_VIEWPORT mainViewport = { 0, 0, static_cast<float>(width), static_cast<float>(height), 0, 1 };
		context->RSSetViewports(1, &mainViewport);
		UnbindSrvs(context.Get());
		context->OMSetRenderTargets(1, ssaoRtv.GetAddressOf(), nullptr);
		context->PSSetShaderResources(0, 1, depthSrv.GetAddressOf());
		BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(ssaoPs.Get()));
		cb.Params.x = 0.03f;
		cb.Params.y = 0.2f;
		uploadCb();
		context->Draw(3, 0);
		cb.Params = XMFLOAT4(0.1f, 0.4f, 1.0f / 256.0f, 0.005f);
		uploadCb();

		FLOAT const hdrClear[4] = { 0, 0, 0, 1 };
		context->ClearRenderTargetView(hdrRtv.Get(), hdrClear);
		UnbindSrvs(context.Get());
		context->OMSetRenderTargets(1, hdrRtv.GetAddressOf(), nullptr);
		ID3D11ShaderResourceView* pbrSrvs[5] = { ssaoSrv.Get(), irrSrv.Get(), specSrv.Get(), brdfSrv.Get(), shadowSrv.Get() };
		context->PSSetShaderResources(0, 5, pbrSrvs);
		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		context->IASetInputLayout(layout.Get());
		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		context->VSSetShader(meshVs, nullptr, 0);
		context->PSSetShader(static_cast<ID3D11PixelShader*>(pbrPs.Get()), nullptr, 0);
		context->Draw(static_cast<UINT>(vertices.size()), 0);

		UnbindSrvs(context.Get());
		context->OMSetRenderTargets(1, ldrRtv.GetAddressOf(), nullptr);
		context->PSSetShaderResources(0, 1, hdrSrv.GetAddressOf());
		BindFullscreen(context.Get(), fullscreenVs, static_cast<ID3D11PixelShader*>(tonePs.Get()));
		context->Draw(3, 0);
		context->Flush();
		return true;
	}
}
