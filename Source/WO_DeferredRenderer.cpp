////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: DeferredRenderer.cpp
///
///			Description:
///
///			Created:	21.02.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_DeferredRenderer.hpp"
#include "WO_DeviceDX11.hpp"
#include "WO_3DComponents.hpp"

namespace WOtech
{
	DeferredRenderer::DeferredRenderer(_In_ DeviceDX11* const& device)
	{
		m_device = device;
		m_beginRender = false;
	}
	void DeferredRenderer::Init()
	{
	}
	DeviceDX11* DeferredRenderer::getDeviceDX11()
	{
		return m_device;
	}
	void DeferredRenderer::Begin()
	{
		if (m_beginRender)
		{
			throw new winrt::hresult_error(); //"Begin was called before End"
		}

		m_CommandQueue.clear();
	}
	void DeferredRenderer::Submit(_In_ Mesh* const& mesh, _In_ Camera* const& camera)
	{
		RenderCommand command;

		command.mesh = mesh;
		command.uniforms.ProjectionMatrix = camera->ProjectionMatrix();
		command.uniforms.ViewMatrix = camera->ViewMatrix();
		command.uniforms.WorldMatrix = mesh->GetWorldMatrix();

		m_CommandQueue.push_back(command);
	}
	void DeferredRenderer::Submit()
	{
	}
	void DeferredRenderer::End()
	{
		if (!m_beginRender)
		{
			throw new winrt::hresult_error(); //"End was called before Begin"
		}
	}
	void DeferredRenderer::Present()
	{
		m_device->Present();
	}
}