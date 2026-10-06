////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_EditorCommand.hpp
///
///			Description:
///			In-process editor operations and the localhost JSON command API.
///			Default port is 27182. One JSON object per line, one JSON
///			response per line. See docs/EditorCommandAPI.md.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_EDITORCOMMAND_H
#define WO_EDITORCOMMAND_H

//////////////
// INCLUDES //
//////////////
#include "WO_Scene.hpp"
#include "WO_Json.hpp"

#include <string>

namespace WOtech
{
	static const UINT16 kEditorCommandPort = 27182;

	class EditorApi
	{
	public:
		explicit EditorApi(_In_ World* world);

		World* getWorld();
		void NewScene();
		bool OpenScene(_In_z_ const char* path, _Out_opt_ std::string* error);
		bool SaveScene(_In_z_ const char* path, _Out_opt_ std::string* error);
		Entity CreateEntity(_In_z_ const char* name);
		bool DestroyEntity(_In_ Entity entity);
		bool AddComponent(_In_ Entity entity, _In_ COMPONENT_TYPE type);
		bool RemoveComponent(_In_ Entity entity, _In_ COMPONENT_TYPE type);
		bool SetTransform(_In_ Entity entity, _In_ Transform const& value);
		bool SetComponentJson(_In_ Entity entity, _In_z_ const char* type, _In_ JsonValue const& fields, _Out_opt_ std::string* error);
		bool ImportGltf(_In_z_ const char* path, _Out_opt_ std::string* error);
		Entity SpawnPrefab(_In_z_ const char* path, _Out_opt_ std::string* error);
		void Play();
		void Stop();
		bool isPlaying() const;
		bool ExportGame(_In_z_ const char* outputDir, _Out_opt_ std::string* error);
		bool Screenshot(_In_z_ const char* path, _Out_opt_ std::string* error);
		std::string QueryScene() const;

		const char* getLastImport() const;
		const char* getLastScreenshot() const;

	private:
		World*			m_world;
		bool			m_playing;
		std::string		m_lastImport;
		std::string		m_lastScreenshot;
		std::string		m_lastError;
	};

	class EditorCommandServer
	{
	public:
		EditorCommandServer();
		~EditorCommandServer();

		EditorCommandServer(_In_ EditorCommandServer const&) = delete;
		EditorCommandServer& operator=(_In_ EditorCommandServer const&) = delete;

		void Attach(_In_ EditorApi* api);
		bool Start(_In_ UINT16 port);
		void Stop();
		void Poll();
		bool isListening() const;
		UINT16 getPort() const;

		std::string Execute(_In_z_ const char* jsonCommand);
		bool ExecuteFile(_In_z_ const char* path, _Out_ std::string* transcript);

	private:
		struct Impl;
		Impl*		m_impl;
		EditorApi*	m_api;
	};
}

#endif
