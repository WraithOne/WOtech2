////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: DefaultFactory.cpp
///
///			Description:
///
///			Created:	27.02.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"

#include "WO_DefaultFactory.hpp"
#include "WO_VertexTypes.hpp"
#include "WO_DeviceDX11.hpp"
#include "WO_3DComponents.hpp"
#include "WO_Materials.hpp"

#include "Shader\Compiled\VS_Basic.inc"
#include "Shader\Compiled\PS_Basic.inc"

namespace WOtech
{
#ifdef WO_WIP
	BasicMaterial* DefaultFactory::CreateBasicMaterial(_In_ WOtech::DeviceDX11* const& device)
	{
		auto vertexshader = new VertexShader((void*)VS_Basic, sizeof(VS_Basic), nullptr, 0, 0, device);
		auto pixelshader = new PixelShader((void*)PS_Basic, sizeof(PS_Basic), device);

		return new BasicMaterial(vertexshader, pixelshader, device);
	}
#endif // WO_WIP

	Mesh* DefaultFactory::CreateCube(_In_ FLOAT const& size, _In_ IMaterial* const& material, _In_ DeviceDX11* const& device)
	{
		VertexPositionColor data[] =
		{
			{ DirectX::XMFLOAT3(-size / 2.0f, -size / 2.0f, -size / 2.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
			{ DirectX::XMFLOAT3(-size / 2.0f, -size / 2.0f, size / 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-size / 2.0f, size / 2.0f, -size / 2.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-size / 2.0f, size / 2.0f, size / 2.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(size / 2.0f, -size / 2.0f, -size / 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(size / 2.0f, -size / 2.0f, size / 2.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(size / 2.0f, size / 2.0f, -size / 2.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(size / 2.0f, size / 2.0f, size / 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
		};

		WORD indices[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		IndexBuffer* iB = new IndexBuffer(indices, sizeof(indices), device);

		UINT32 stride = sizeof(VertexPositionColor);
		VertexBuffer* vB = new VertexBuffer(data, 8 * data->Sizeof(), stride, 0U, device);

		return new Mesh(vB, iB, material, PCVertexLayout);
	}
	Mesh* DefaultFactory::CreateTriangle(_In_ FLOAT const& size, _In_ IMaterial* const& material, _In_ DeviceDX11* const& device)
	{
		UNREFERENCED_PARAMETER(size);

		VertexPositionColor data[] =
		{
			{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
		};

		WORD indices[] =
		{
			0, 1, 2
		};

		IndexBuffer* iB = new IndexBuffer(indices, sizeof(indices), device);
		UINT32 stride = sizeof(VertexPositionColor);
		VertexBuffer* vB = new VertexBuffer(data, 3 * data->Sizeof(), stride, 0U, device);

		return new Mesh(vB, iB, material, PCVertexLayout);
	}
}