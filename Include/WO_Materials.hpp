////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Materials.h
///
///			Description:
///
///			Created:	20.08.2017
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_MATERIALS_H
#define WO_MATERIALS_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	///////////////////////
	// Forward declarations
	///////////////////////
	class DeviceDX11;
	class VertexShader;
	class PixelShader;

	// Stores the vector and Color�s for a Directional light
	struct DirectionalLight
	{
		DirectionalLight();

		DirectX::XMVECTOR Direction;
		DirectX::XMVECTOR DiffuseColor;
		DirectX::XMVECTOR SpecularColor;
	};

	// Strores the Matrices for Materials
	struct MaterialMatrices
	{
		MaterialMatrices();

		DirectX::XMMATRIX World;
		DirectX::XMMATRIX WorldInverse;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
		DirectX::XMMATRIX worldView;
	};

	// Stores the Materials Color
	struct MaterialColor
	{
		MaterialColor();

		DirectX::XMVECTOR DiffuseColor;
		FLOAT Alpha;
	};

	// Stores the Ambient and Directional Lights for Material
	struct MaterialLights : public MaterialColor
	{
		MaterialLights();

		static const INT MaxDirectionalLights = 3;

		DirectX::XMVECTOR EmissiveColor;
		DirectX::XMVECTOR AmbientLightColor;

		bool LightEnabled[MaxDirectionalLights];
		WOtech::DirectionalLight DirectionalLights[MaxDirectionalLights];
	};

	// Constant buffer layout. Must match the shader!
	struct MaterialConstants
	{
		DirectX::XMVECTOR diffuseColor;
		DirectX::XMVECTOR emissiveColor;
		DirectX::XMVECTOR specularColorAndPower;

		DirectX::XMVECTOR lightDirection[WOtech::MaterialLights::MaxDirectionalLights];
		DirectX::XMVECTOR lightDiffuseColor[WOtech::MaterialLights::MaxDirectionalLights];
		DirectX::XMVECTOR lightSpecularColor[WOtech::MaterialLights::MaxDirectionalLights];

		DirectX::XMVECTOR eyePosition;

		DirectX::XMVECTOR fogColor;
		DirectX::XMVECTOR fogVector;

		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInverse;
		DirectX::XMVECTOR worldInverseTranspose[3];
		DirectX::XMMATRIX worldViewProj;
	};

	interface IMaterial
	{
	public:
		virtual void bindMaterial(_In_ WOtech::DeviceDX11* const& device) = 0;
		virtual void unbindMaterial(_In_ WOtech::DeviceDX11* const& device) = 0;
	};

	// Interface for Materials with World,View and Projection Matrices
	interface IMaterialMatrices
	{
	public:
		virtual void setWorld(_In_ DirectX::XMFLOAT4X4 const& world) = 0;
		virtual void setWorldInverse(_In_ DirectX::XMFLOAT4X4 const& worldInverse) = 0;
		virtual void setView(_In_ DirectX::XMFLOAT4X4 const& view) = 0;
		virtual void setProjection(_In_ DirectX::XMFLOAT4X4 const& projection) = 0;

		virtual void XM_CALLCONV getMatrices(_Out_ DirectX::XMMATRIX* worldViewProjectionConstant, _Out_ DirectX::XMMATRIX* worldConstant, _Out_ DirectX::XMMATRIX* worldInverseConstant) = 0;
	};

	// Interface for Materials with Directional Lightning
	interface IMaterialLights
	{
	public:
		virtual void SetLightingEnabled(_In_ bool const& value) = 0;
		virtual void SetPerPixelLighting(_In_ bool const& value) = 0;
		virtual void SetAmbientLightColor(_In_ DirectX::XMFLOAT4 const& value) = 0;

		virtual void SetLightEnabled(_In_ INT const& whichLight, _In_ bool const& value) = 0;
		virtual void SetLightDirection(_In_ INT const& whichLight, _In_ DirectX::XMFLOAT4 const& value) = 0;
		virtual void SetLightDiffuseColor(_In_ INT const& whichLight, _In_ DirectX::XMFLOAT4 const& value) = 0;
		virtual void SetLightSpecularColor(_In_ INT const& whichLight, _In_ DirectX::XMFLOAT4 const& value) = 0;

		virtual void EnableDefaultLighting() = 0;
	};

	class BasicMaterial : public IMaterial, IMaterialMatrices
	{
	public:
		BasicMaterial(_In_ WOtech::VertexShader* const& Vshader, _In_ WOtech::PixelShader* const& Pshader, _In_ WOtech::DeviceDX11* const& device);

		void bindMaterial(_In_ WOtech::DeviceDX11* const& device);
		void unbindMaterial(_In_ WOtech::DeviceDX11* const& device);

		void setWorld(_In_ DirectX::XMFLOAT4X4 const& world);
		void setWorldInverse(_In_ DirectX::XMFLOAT4X4 const& worldInverse);
		void setView(_In_ DirectX::XMFLOAT4X4 const& view);
		void setProjection(_In_ DirectX::XMFLOAT4X4 const& projection);
		void XM_CALLCONV getMatrices(_Out_ DirectX::XMMATRIX* worldViewProjectionConstant, _Out_ DirectX::XMMATRIX* worldConstant, _Out_ DirectX::XMMATRIX* worldInverseConstant);

	private:
		void setConstantBuffer(_In_ WOtech::DeviceDX11* const& device);

	private:
		WOtech::VertexShader*					m_vertexShader;
		WOtech::PixelShader*					m_pixelShader;

		WOtech::MaterialMatrices				m_matrices;

		WOtech::MaterialConstants				m_constants;

		// Uniform buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_constantBuffer;
	};
}
#endif // WO_MATERIALS_H
