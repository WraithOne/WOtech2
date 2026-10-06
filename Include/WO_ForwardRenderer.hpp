////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: ForwardRenderer.h
///
///			Description:
///
///			Created:	07.05.2014
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_FORWARDRENDERER_H
#define WO_FORWARDRENDERER_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "..\Include\WO_IRenderer.hpp"
#include "..\Include\WO_RenderCommand.hpp"

namespace WOtech
{
	// Forward declaration
	class WOtech::DeviceDX11;

	class ForwardRenderer : public WOtech::IRenderer
	{
	public:
		ForwardRenderer(_In_ WOtech::DeviceDX11* const& device);

		virtual void Begin();
		virtual void Submit(_In_ WOtech::Mesh* const& mesh, _In_ WOtech::Camera* const& camera);
		virtual void Submit();
		virtual void End();
		virtual void Present();

		virtual WOtech::DeviceDX11* getDeviceDX11();

		virtual void Init();

	private:
		WOtech::DeviceDX11*		m_device;

		bool					m_beginRender;

		// Render command queue
		WOtech::CommandQueue	m_CommandQueue;
	};
}
#endif
