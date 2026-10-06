////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: IRenderer.h
///
///			Description:
///
///			Created:	07.05.2014
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_IRENDERER_H
#define WO_IRENDERER_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "..\Include\WO_VertexTypes.hpp"

namespace WOtech
{
	// Forward decl.
	class Mesh;
	class Camera;
	class DeviceDX11;

	interface IRenderer
	{
	public:
		virtual void Begin() = 0;
		virtual void Submit(_In_ WOtech::Mesh* const& mesh, _In_ WOtech::Camera* const& camera) = 0;
		virtual void Submit() = 0;
		virtual void End() = 0;
		virtual void Present() = 0;

		virtual WOtech::DeviceDX11* getDeviceDX11() = 0;

		virtual void Init() = 0;
	};
}
#endif
