////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_RenderPipeline.hpp
///
///			Description:
///			HDR, tonemap, SSAO, PCF shadows, and IBL sampled by PBR.
///			Does not replace DeviceDX11. Swap chain stays B8G8R8A8.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_RENDERPIPELINE_H
#define WO_RENDERPIPELINE_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

#include <string>

namespace WOtech
{
	class RenderPipeline
	{
	public:
		static bool RenderFeatureFrame(_Out_opt_ std::string* error);
	};
}

#endif
