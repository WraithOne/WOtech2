////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: VertexTypes.h
///
///			Description:
///
///			Created:	25.03.2017
///			Edited:		30.11.2025
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_VERTEXTYPES_H
#define WO_VERTEXTYPES_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position and Color
	/////////////////////////////////////////////////////////////////

	struct VertexPositionColor
	{
	public:
		VertexPositionColor() : Position(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f) {}
		VertexPositionColor(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT4 const& color) : Position(position), Color(color) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT4)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

	static D3D11_INPUT_ELEMENT_DESC PCVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position and Texture
	/////////////////////////////////////////////////////////////////

	struct VertexPositionTexture
	{
	public:
		VertexPositionTexture() : Position(0.0f, 0.0f, 0.0f), TextureCoordinate(0.0f, 0.0f) {}
		VertexPositionTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT2 const& texCoord) : Position(position), TextureCoordinate(texCoord) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT2)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	static D3D11_INPUT_ELEMENT_DESC PTVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position and Normal
	/////////////////////////////////////////////////////////////////

	struct VertexPositionNormal
	{
	public:
		VertexPositionNormal() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f) {}
		VertexPositionNormal(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal) : Position(position), Normal(normal) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT3)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
	};

	static D3D11_INPUT_ELEMENT_DESC PNVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position, Normal and Color
	/////////////////////////////////////////////////////////////////

	struct VertexPositionNormalColor
	{
	public:
		VertexPositionNormalColor() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f) {}
		VertexPositionNormalColor(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT4 const& color) : Position(position), Normal(normal), Color(color) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT4)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT4 Color;
	};

	static D3D11_INPUT_ELEMENT_DESC PNCVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position, Color and Texture
	/////////////////////////////////////////////////////////////////

	struct VertexPositionColorTexture
	{
	public:
		VertexPositionColorTexture() : Position(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), TextureCoordinate(0.0f, 0.0f) {}
		VertexPositionColorTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT4 const& color, DirectX::XMFLOAT2 const& texCoord) : Position(position), Color(color), TextureCoordinate(texCoord) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT2)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	static D3D11_INPUT_ELEMENT_DESC PCTVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position, Noraml and Texture
	/////////////////////////////////////////////////////////////////

	struct VertexPositionNormalTexture
	{
	public:
		VertexPositionNormalTexture() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f), TextureCoordinate(0.0f, 0.0f) {}
		VertexPositionNormalTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& texCoord) : Position(position), Normal(normal), TextureCoordinate(texCoord) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT2)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	static D3D11_INPUT_ELEMENT_DESC PNTVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	/////////////////////////////////////////////////////////////////
	/////////// Vertex with Position, Normal, Color and Texture
	/////////////////////////////////////////////////////////////////

	struct VertexPositionNormalColorTexture
	{
	public:
		VertexPositionNormalColorTexture() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), TextureCoordinate(0.0f, 0.0f) {}
		VertexPositionNormalColorTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT4 const& color, DirectX::XMFLOAT2 const& texCoord) : Position(position), Normal(normal), Color(color), TextureCoordinate(texCoord) {}

		static UINT Sizeof() { return (sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2)); }

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	static D3D11_INPUT_ELEMENT_DESC PNCTVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
}
#endif