////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Scene.hpp
///
///			Description:
///			Versioned JSON scene and prefab documents.
///			format "wotech2-scene" or "wotech2-prefab", version 1.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_SCENE_H
#define WO_SCENE_H

//////////////
// INCLUDES //
//////////////
#include "WO_World.hpp"

#include <string>

namespace WOtech
{
	static const INT kSceneVersion = 1;

	class SceneDocument
	{
	public:
		static std::string ToJson(_In_ World const& world);
		static bool FromJson(_Inout_ World* world, _In_z_ const char* json, _Out_opt_ std::string* error);

		static bool Save(_In_ World const& world, _In_z_ const char* path, _Out_opt_ std::string* error);
		static bool Load(_Inout_ World* world, _In_z_ const char* path, _Out_opt_ std::string* error);

		static std::string PrefabToJson(_In_ Prefab const& prefab);
		static bool PrefabFromJson(_Out_ Prefab* prefab, _In_z_ const char* json, _Out_opt_ std::string* error);
		static bool SavePrefab(_In_ Prefab const& prefab, _In_z_ const char* path, _Out_opt_ std::string* error);
		static bool LoadPrefab(_Out_ Prefab* prefab, _In_z_ const char* path, _Out_opt_ std::string* error);
	};
}

#endif
