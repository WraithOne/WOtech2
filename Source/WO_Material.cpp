////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Material.cpp
///
///			Description:
///
///			Created:	25.02.2016
///			Edited:		01.05.2018
///
////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Materials.hpp"
#include "WO_DeviceDX11.hpp"
#include "WO_3DComponents.hpp"
#include "WO_Utilities.hpp"

#ifdef WO_WIP

namespace WOtech
{
	///////////////////////
	// Struct DirectionalLight
	///////////////////////
	WOtech::DirectionalLight::DirectionalLight()
	{
		Direction = DirectX::g_XMOne;
		DiffuseColor = DirectX::g_XMOne;
		SpecularColor = DirectX::g_XMOne;
	}

	///////////////////////
	// Struct MaterialMatrices
	///////////////////////
	MaterialMatrices::MaterialMatrices()
	{
		World = DirectX::XMMatrixIdentity();
		WorldInverse = DirectX::XMMatrixIdentity();
		View = DirectX::XMMatrixIdentity();
		Projection = DirectX::XMMatrixIdentity();
		worldView = DirectX::XMMatrixIdentity();
	}

	///////////////////////
	// Struct MaterialColor
	///////////////////////
	MaterialColor::MaterialColor()
	{
		Alpha = 1.0f;
		DiffuseColor = DirectX::g_XMOne;
	}

	///////////////////////
	// Struct MaterialLights
	///////////////////////
	MaterialLights::MaterialLights()
	{
		EmissiveColor = DirectX::g_XMZero;
		AmbientLightColor = DirectX::g_XMZero;

		for (INT i = 0; i < MaxDirectionalLights; i++)
		{
			LightEnabled[i] = 0;
		}
	}

	///////////////////////
	// BasicMaterial
	///////////////////////
	BasicMaterial::BasicMaterial(_In_ VertexShader* const& Vshader, _In_ PixelShader* const& Pshader, _In_ DeviceDX11* const& device)
	{
		m_constants = { 0 };
		m_vertexShader = Vshader;
		m_pixelShader = Pshader;

		HRESULT hr;
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(MaterialConstants);
		constDesc.Usage = D3D11_USAGE_DEFAULT;
		constDesc.CPUAccessFlags = 0;
		constDesc.StructureByteStride = 0;
		constDesc.MiscFlags = 0;

		auto DXdevice = device->getDevice();
		hr = DXdevice->CreateBuffer(&constDesc, nullptr, &m_constantBuffer);
		ThrowIfFailed(hr);
	}

	void BasicMaterial::bindMaterial(_In_ DeviceDX11* const& device)
	{
		auto context = device->getContext();

		// Submit Uniforms
		setConstantBuffer(device);

		// Set VertexShader
		context->IASetInputLayout(m_vertexShader->getInputLayout());
		context->VSSetShader(m_vertexShader->getShader(), nullptr, 0);

		// Set PixelShader
		context->PSSetShader(m_pixelShader->getShader(), nullptr, 0);
	}
	void BasicMaterial::unbindMaterial(_In_ DeviceDX11* const& device)
	{
		auto context = device->getContext();

		context->IASetInputLayout(nullptr);
		context->VSSetShader(nullptr, 0, 0);

		context->PSSetShader(nullptr, 0, 0);
	}

	void BasicMaterial::setWorld(_In_ DirectX::XMFLOAT4X4 const& world)
	{
		m_matrices.World = DirectX::XMLoadFloat4x4(&world);
	}
	void BasicMaterial::setWorldInverse(_In_ DirectX::XMFLOAT4X4 const& worldInverse)
	{
		m_matrices.WorldInverse = DirectX::XMLoadFloat4x4(&worldInverse);
	}
	void BasicMaterial::setView(_In_ DirectX::XMFLOAT4X4 const& view)
	{
		m_matrices.View = DirectX::XMLoadFloat4x4(&view);
	}
	void BasicMaterial::setProjection(_In_ DirectX::XMFLOAT4X4 const& projection)
	{
		m_matrices.Projection = DirectX::XMLoadFloat4x4(&projection);
	}

	void XM_CALLCONV BasicMaterial::setConstants(_Out_ DirectX::XMMATRIX worldViewProjectionConstant, _Out_ DirectX::XMMATRIX worldConstant, _Out_ DirectX::XMMATRIX worldInverseConstant)
	{
		worldConstant = DirectX::XMLoadFloat4x4(&world);

		m_matrices.View = DirectX::XMLoadFloat4x4(&view);
		worldViewProjectionConstant = DirectX::XMLoadFloat4x4(&projection);
	}

	void XM_CALLCONV WOtech::BasicMaterial::setConstantBuffer(_In_ DeviceDX11* const& device)
	{
		m_matrices.setConstants(&m_constants.worldViewProj, &m_constants.world, &m_constants.worldInverse);
		auto context = device->getContext();

		// Update Constantbuffer
		context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &m_constants, 0, 0, 0);

		// set Constantbuffer
		context->CSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	}

	void XM_CALLCONV WOtech::BasicMaterial::getMatrices(_Out_ DirectX::XMMATRIX* worldViewProjectionConstant, _Out_ DirectX::XMMATRIX* worldConstant, _Out_ DirectX::XMMATRIX* worldInverseConstant)
	{
		*worldViewProjectionConstant = m_matrices.World * m_matrices.View * m_matrices.Projection;
		*worldConstant = m_matrices.World;
		*worldInverseConstant = m_matrices.WorldInverse;
	}
}

#endif // WO_WIP