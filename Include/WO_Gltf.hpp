////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Gltf.hpp
///
///			Description:
///			glTF 2.0 import of meshes, materials, and texture URIs.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_GLTF_H
#define WO_GLTF_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

#include <string>

namespace WOtech
{
	struct GltfImportResult
	{
		UINT32 MeshCount;
		UINT32 MaterialCount;
		UINT32 TextureCount;
		std::string Name;
	};

	class GltfImporter
	{
	public:
		static bool Load(_In_z_ const char* path, _Out_ GltfImportResult* result, _Out_opt_ std::string* error);
	};
}

#endif
