////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: ForwardRenderer.cpp
///
///			Description:
///
///			Created:	21.02.2016
///			Edited:		05.01.2021
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_3DComponents.hpp"
#include "WO_Materials.hpp"
#include "WO_ForwardRenderer.hpp"
#include "WO_DeviceDX11.hpp"
#include "WO_Utilities.hpp"

namespace WOtech
{
	ForwardRenderer::ForwardRenderer(_In_ DeviceDX11* const& device)
	{
		m_device = device;
		m_beginRender = false;
	}
	void ForwardRenderer::Begin()
	{
		if (m_beginRender)
		{
			throw winrt::hresult_invalid_argument{ L"Begin was called before End" };
		}
		else
		{
			m_beginRender = true;
		}
		m_CommandQueue.clear();
	}
	void ForwardRenderer::Submit(_In_ Mesh* const& mesh, _In_ Camera* const& camera)
	{
		RenderCommand command;

		command.mesh = mesh;
		command.uniforms.ProjectionMatrix = camera->ProjectionMatrix();
		command.uniforms.ViewMatrix = camera->ViewMatrix();
		command.uniforms.WorldMatrix = mesh->GetWorldMatrix();

		m_CommandQueue.push_back(command);
	}
	void ForwardRenderer::Submit()
	{
		// for lights
	}
	void ForwardRenderer::End()
	{
		if (!m_beginRender)
		{
			throw winrt::hresult_invalid_argument{ L"End was called before Begin" };
		}
		else
		{
			m_beginRender = false;
		}

		// do sorting here

		for (UINT i = 0; i < m_CommandQueue.size(); i++)
		{
			// Submit Shaders, Textures
			const RenderCommand& command = m_CommandQueue[i];
#ifdef WO_WIP
			auto materialMatrices = command.mesh->GetMaterial();
			if (materialMatrices)
				materialMatrices->setMatrices(command.uniforms.ProjectionMatrix, command.uniforms.ViewMatrix, command.uniforms.WorldInverseMatrix);
#endif // WO_WIP
			command.mesh->bindMaterial(m_device);

			// Submit Vertex and Index data and render the final mesh
			command.mesh->Render(m_device);
		}
	}
	void ForwardRenderer::Present()
	{
		m_device->Present();
	}

	void ForwardRenderer::Init()
	{
	}

	DeviceDX11* ForwardRenderer::getDeviceDX11()
	{
		return m_device;
	}
}