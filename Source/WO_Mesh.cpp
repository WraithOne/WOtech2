////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Mesh.cpp
///
///			Description:
///
///			Created:	23.02.2016
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_3DComponents.hpp"
#include "WO_Materials.hpp"
#include "WO_DeviceDX11.hpp"

namespace WOtech
{
	Mesh::Mesh(_In_ VertexBuffer* const& vertexBuffer, _In_ IndexBuffer* const& indexBuffer, _In_ IMaterial* const& material, _In_ D3D11_INPUT_ELEMENT_DESC* const& inputLayout)
	{
		m_vertexBuffer = vertexBuffer;
		m_indexBuffer = indexBuffer;
		m_material = material;
		m_inputLayout = inputLayout;

		m_position = { 0.0f, 0.0f, 0.0f };
		m_scaling = { 0.0f, 0.0f, 0.0f };
		m_rotation = { 0.0f, 0.0f, 0.0f };
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::setGeometry(_In_ WOtech::VertexBuffer* const& vertex, _In_ WOtech::IndexBuffer* const& index)
	{ 
		m_vertexBuffer = vertex, m_indexBuffer = index; 
	}
	void Mesh::setMaterial(_In_ WOtech::IMaterial* const& material)
	{ 
		m_material = material;
	}
	void Mesh::setInputLayout(_In_ D3D11_INPUT_ELEMENT_DESC* const& inputLayout)
	{ 
		m_inputLayout = inputLayout;
	}
	void Mesh::setPosition(_In_  DirectX::XMFLOAT3 const& position)
	{ 
		m_position = position; 
	}
	void Mesh::setScaling(_In_ DirectX::XMFLOAT3 const& scaling)
	{ m_scaling = scaling;
	}
	void Mesh::setRotation(_In_ DirectX::XMFLOAT3 const& rotation)
	{ 
		m_rotation = rotation;
	}

	void Mesh::bindMaterial(_In_ DeviceDX11* device)
	{
		UNREFERENCED_PARAMETER(device);

#ifdef WO_WIP
		m_material->bindMaterial(device);
#endif
	}

	void Mesh::Render(_In_ DeviceDX11* device)
	{
		m_vertexBuffer->SubmitBuffer(device);
		m_indexBuffer->SubmitBuffer(device);

		auto context = device->getContext();

		context->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
	}

	const DirectX::XMFLOAT3	Mesh::getPosition()
	{ 
		return m_position; 
	}
	const DirectX::XMFLOAT3	Mesh::getScaling()
	{ 
		return m_scaling; 
	}
	const DirectX::XMFLOAT3	Mesh::getRotation()
	{ 
		return m_rotation; 
	}
	const DirectX::XMMATRIX	Mesh::GetWorldMatrix()
	{
		return DirectX::XMMatrixTransformation(DirectX::g_XMZero, DirectX::XMQuaternionIdentity(), DirectX::XMLoadFloat3(&m_scaling), DirectX::g_XMZero, DirectX::XMLoadFloat3(&m_rotation), DirectX::XMLoadFloat3(&m_position));
	}
	const WOtech::IMaterial* Mesh::GetMaterial()
	{ 
		return m_material; 
	}
	const D3D11_INPUT_ELEMENT_DESC* Mesh::getInputLayout()
	{ 
		return m_inputLayout; 
	}
}// namespace WOtech