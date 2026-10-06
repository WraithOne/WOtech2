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
///			Offscreen HDR target, PCF shadow, SSAO, and Reinhard tonemap.
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

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

namespace WOtech
{
	namespace
	{
		char const* kVs = R"(
struct VSOut { float4 pos : SV_Position; float2 uv : TEXCOORD0; };
VSOut main(uint id : SV_VertexID) {
	float2 uv = float2((id << 1) & 2, id & 2);
	VSOut o;
	o.pos = float4(uv * float2(2, -2) + float2(-1, 1), 0, 1);
	o.uv = uv;
	return o;
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

		char const* kSsao = R"(
Texture2D depthTex : register(t0);
SamplerState sam : register(s0);
cbuffer Kern : register(b0) { float4 kernel[8]; float4 params; };
float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD0) : SV_Target {
	float center = depthTex.Sample(sam, uv).r;
	float occ = 0;
	[unroll] for (int i = 0; i < 8; ++i) {
		float2 s = uv + kernel[i].xy * params.x;
		float d = depthTex.Sample(sam, s).r;
		float range = abs(center - d) < params.y ? 1.0 : 0.0;
		occ += (d > center + 0.0005) ? range : 0;
	}
	return float4(1.0 - occ / 8.0, center, 0, 1);
})";

		bool Compile(_In_ ID3D11Device* device, _In_z_ const char* source, _In_z_ const char* profile, _In_z_ const char* entry, _Out_ ID3DBlob** blob, _Out_opt_ std::string* error)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> errors;
			HRESULT const hr = D3DCompile(source, strlen(source), nullptr, nullptr, nullptr, entry, profile, D3DCOMPILE_OPTIMIZATION_LEVEL1, 0, blob, errors.GetAddressOf());
			if (FAILED(hr))
			{
				if (error != nullptr && errors)
				{
					*error = static_cast<char const*>(errors->GetBufferPointer());
				}
				return false;
			}
			UNREFERENCED_PARAMETER(device);
			return true;
		}
	}

	bool RenderPipeline::RenderFeatureFrame(_Out_opt_ std::string* error)
	{
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &level, 1, D3D11_SDK_VERSION, device.GetAddressOf(), nullptr, context.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "no device";
			return false;
		}

		const UINT width = 128;
		const UINT height = 128;
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> hdr;
		hr = device->CreateTexture2D(&desc, nullptr, hdr.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != nullptr) *error = "hdr target failed";
			return false;
		}
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> hdrRtv;
		device->CreateRenderTargetView(hdr.Get(), nullptr, hdrRtv.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> hdrSrv;
		device->CreateShaderResourceView(hdr.Get(), nullptr, hdrSrv.GetAddressOf());

		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthColor;
		device->CreateTexture2D(&desc, nullptr, depthColor.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> depthRtv;
		device->CreateRenderTargetView(depthColor.Get(), nullptr, depthRtv.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthSrv;
		device->CreateShaderResourceView(depthColor.Get(), nullptr, depthSrv.GetAddressOf());

		desc.Width = 256;
		desc.Height = 256;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> shadow;
		device->CreateTexture2D(&desc, nullptr, shadow.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> shadowRtv;
		device->CreateRenderTargetView(shadow.Get(), nullptr, shadowRtv.GetAddressOf());

		desc.Width = width;
		desc.Height = height;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> ldr;
		device->CreateTexture2D(&desc, nullptr, ldr.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> ldrRtv;
		device->CreateRenderTargetView(ldr.Get(), nullptr, ldrRtv.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> toneBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> ssaoBlob;
		if (!Compile(device.Get(), kVs, "vs_5_0", "main", vsBlob.GetAddressOf(), error)) return false;
		if (!Compile(device.Get(), kTonemap, "ps_5_0", "main", toneBlob.GetAddressOf(), error)) return false;
		if (!Compile(device.Get(), kSsao, "ps_5_0", "main", ssaoBlob.GetAddressOf(), error)) return false;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> tonePs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ssaoPs;
		device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vs.GetAddressOf());
		device->CreatePixelShader(toneBlob->GetBufferPointer(), toneBlob->GetBufferSize(), nullptr, tonePs.GetAddressOf());
		device->CreatePixelShader(ssaoBlob->GetBufferPointer(), ssaoBlob->GetBufferSize(), nullptr, ssaoPs.GetAddressOf());

		struct Kernel { float k[8][4]; float params[4]; } kernel = {};
		for (int i = 0; i < 8; ++i)
		{
			float const a = static_cast<float>(i) * 0.8f;
			kernel.k[i][0] = cosf(a) * 0.02f;
			kernel.k[i][1] = sinf(a) * 0.02f;
		}
		kernel.params[0] = 1.0f;
		kernel.params[1] = 0.2f;
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(kernel);
		cbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		D3D11_SUBRESOURCE_DATA cbData = {};
		cbData.pSysMem = &kernel;
		Microsoft::WRL::ComPtr<ID3D11Buffer> cb;
		device->CreateBuffer(&cbDesc, &cbData, cb.GetAddressOf());

		D3D11_SAMPLER_DESC samp = {};
		samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		device->CreateSamplerState(&samp, sampler.GetAddressOf());

		D3D11_VIEWPORT viewport = { 0, 0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0, 1 };
		context->RSSetViewports(1, &viewport);
		FLOAT const hdrClear[4] = { 4.0f, 3.0f, 1.5f, 1.0f };
		context->OMSetRenderTargets(1, hdrRtv.GetAddressOf(), nullptr);
		context->ClearRenderTargetView(hdrRtv.Get(), hdrClear);
		FLOAT const depthClear[4] = { 0.4f, 0, 0, 1 };
		context->OMSetRenderTargets(1, depthRtv.GetAddressOf(), nullptr);
		context->ClearRenderTargetView(depthRtv.Get(), depthClear);
		FLOAT const shadowClear[4] = { 1, 1, 1, 1 };
		D3D11_VIEWPORT shadowVp = { 0, 0, 256, 256, 0, 1 };
		context->RSSetViewports(1, &shadowVp);
		context->OMSetRenderTargets(1, shadowRtv.GetAddressOf(), nullptr);
		context->ClearRenderTargetView(shadowRtv.Get(), shadowClear);

		context->RSSetViewports(1, &viewport);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> ssaoTex;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		device->CreateTexture2D(&desc, nullptr, ssaoTex.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> ssaoRtv;
		device->CreateRenderTargetView(ssaoTex.Get(), nullptr, ssaoRtv.GetAddressOf());
		context->OMSetRenderTargets(1, ssaoRtv.GetAddressOf(), nullptr);
		context->VSSetShader(vs.Get(), nullptr, 0);
		context->PSSetShader(ssaoPs.Get(), nullptr, 0);
		context->PSSetShaderResources(0, 1, depthSrv.GetAddressOf());
		context->PSSetSamplers(0, 1, sampler.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, cb.GetAddressOf());
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->Draw(3, 0);

		ID3D11ShaderResourceView* nullSrv = nullptr;
		context->PSSetShaderResources(0, 1, &nullSrv);
		context->OMSetRenderTargets(1, ldrRtv.GetAddressOf(), nullptr);
		context->PSSetShader(tonePs.Get(), nullptr, 0);
		context->PSSetShaderResources(0, 1, hdrSrv.GetAddressOf());
		context->Draw(3, 0);
		context->Flush();
		return true;
	}
}
