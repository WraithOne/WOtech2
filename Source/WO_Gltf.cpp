////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Gltf.cpp
///
///			Description:
///			cgltf-backed glTF 2.0 reader.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Gltf.hpp"

#pragma warning(push)
#pragma warning(disable: 4996)
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#pragma warning(pop)

namespace WOtech
{
	bool GltfImporter::Load(_In_z_ const char* path, _Out_ GltfImportResult* result, _Out_opt_ std::string* error)
	{
		if (result == nullptr || path == nullptr)
		{
			if (error != nullptr) *error = "missing gltf arguments";
			return false;
		}
		cgltf_options options = {};
		cgltf_data* data = nullptr;
		cgltf_result const parsed = cgltf_parse_file(&options, path, &data);
		if (parsed != cgltf_result_success || data == nullptr)
		{
			if (error != nullptr) *error = "gltf parse failed";
			return false;
		}
		cgltf_load_buffers(&options, data, path);
		result->MeshCount = static_cast<UINT32>(data->meshes_count);
		result->MaterialCount = static_cast<UINT32>(data->materials_count);
		result->TextureCount = static_cast<UINT32>(data->textures_count);
		result->Name = (data->meshes_count > 0 && data->meshes[0].name != nullptr) ? data->meshes[0].name : "mesh";
		cgltf_free(data);
		return result->MeshCount > 0;
	}
}
