////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: RenderCommand.h
///
///			Description:
///
///			Created:	27.02.2016
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_RENDERCOMMAND_H
#define WO_RENDERCOMMAND_H
//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	class WOtech::Mesh;

	struct RendererUniforms
	{
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX WorldInverseMatrix;
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ProjectionMatrix;
	};

	struct RenderCommand
	{
		WOtech::Mesh* mesh;
		WOtech::RendererUniforms uniforms;
	};

	typedef std::vector<WOtech::RenderCommand> CommandQueue;
}
#endif
