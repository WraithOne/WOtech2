////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_EditorCommand.cpp
///
///			Description:
///			Editor operations used by both the command server and the UI.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_EditorCommand.hpp"
#include "WO_Json.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <fstream>
#include <sstream>

namespace WOtech
{
	namespace
	{
		std::string OkEntity(_In_ Entity entity)
		{
			JsonValue root;
			root.Set("ok").setBool(true);
			char id[32];
			sprintf_s(id, "%llu", static_cast<unsigned long long>(entity));
			root.Set("id").setString(id);
			return root.Stringify();
		}

		std::string OkMessage(_In_z_ const char* message)
		{
			JsonValue root;
			root.Set("ok").setBool(true);
			root.Set("message").setString(message != nullptr ? message : "");
			return root.Stringify();
		}

		std::string Fail(_In_z_ const char* message)
		{
			JsonValue root;
			root.Set("ok").setBool(false);
			root.Set("error").setString(message != nullptr ? message : "error");
			return root.Stringify();
		}

		Entity ParseId(_In_ JsonValue const& command)
		{
			JsonValue const* id = command.Find("id");
			if (id == nullptr)
			{
				return kInvalidEntity;
			}
			if (id->isString())
			{
				return static_cast<Entity>(_strtoui64(id->getString(), nullptr, 10));
			}
			return static_cast<Entity>(id->getNumber(0.0));
		}

		bool ReadFloat3(_In_ JsonValue const* value, _Out_ DirectX::XMFLOAT3* out)
		{
			if (value == nullptr || !value->isArray() || value->getCount() < 3 || out == nullptr)
			{
				return false;
			}
			out->x = static_cast<FLOAT>(value->getAt(0)->getNumber(0.0));
			out->y = static_cast<FLOAT>(value->getAt(1)->getNumber(0.0));
			out->z = static_cast<FLOAT>(value->getAt(2)->getNumber(0.0));
			return true;
		}

		COMPONENT_TYPE ParseType(_In_z_ const char* name)
		{
			if (name == nullptr) return COMPONENT_TYPE_NONE;
			if (strcmp(name, "transform") == 0) return COMPONENT_TYPE_TRANSFORM;
			if (strcmp(name, "camera") == 0) return COMPONENT_TYPE_CAMERA;
			if (strcmp(name, "meshRenderer") == 0) return COMPONENT_TYPE_MESH_RENDERER;
			if (strcmp(name, "rigidBody") == 0) return COMPONENT_TYPE_RIGID_BODY;
			if (strcmp(name, "collider") == 0) return COMPONENT_TYPE_COLLIDER;
			if (strcmp(name, "audioSource") == 0) return COMPONENT_TYPE_AUDIO_SOURCE;
			if (strcmp(name, "script") == 0) return COMPONENT_TYPE_SCRIPT;
			if (strcmp(name, "light") == 0) return COMPONENT_TYPE_LIGHT;
			if (strcmp(name, "prefab") == 0) return COMPONENT_TYPE_PREFAB;
			return COMPONENT_TYPE_NONE;
		}
	}

	EditorApi::EditorApi(_In_ World* world)
		: m_world(world)
		, m_playing(false)
	{
	}

	World* EditorApi::getWorld()
	{
		return m_world;
	}

	void EditorApi::NewScene()
	{
		if (m_world != nullptr)
		{
			m_world->Clear();
		}
		m_playing = false;
	}

	bool EditorApi::OpenScene(_In_z_ const char* path, _Out_opt_ std::string* error)
	{
		return m_world != nullptr && SceneDocument::Load(m_world, path, error);
	}

	bool EditorApi::SaveScene(_In_z_ const char* path, _Out_opt_ std::string* error)
	{
		return m_world != nullptr && SceneDocument::Save(*m_world, path, error);
	}

	Entity EditorApi::CreateEntity(_In_z_ const char* name)
	{
		return m_world != nullptr ? m_world->CreateEntity(name) : kInvalidEntity;
	}

	bool EditorApi::DestroyEntity(_In_ Entity entity)
	{
		if (m_world == nullptr || !m_world->isAlive(entity))
		{
			return false;
		}
		m_world->DestroyEntity(entity);
		return true;
	}

	bool EditorApi::AddComponent(_In_ Entity entity, _In_ COMPONENT_TYPE type)
	{
		if (m_world == nullptr || !m_world->isAlive(entity))
		{
			return false;
		}
		switch (type)
		{
		case COMPONENT_TYPE_TRANSFORM: return m_world->AddTransform(entity, Transform()) != nullptr;
		case COMPONENT_TYPE_CAMERA: return m_world->AddCamera(entity, CameraComponent()) != nullptr;
		case COMPONENT_TYPE_MESH_RENDERER: return m_world->AddMeshRenderer(entity, MeshRenderer()) != nullptr;
		case COMPONENT_TYPE_RIGID_BODY: return m_world->AddRigidBody(entity, RigidBody()) != nullptr;
		case COMPONENT_TYPE_COLLIDER: return m_world->AddCollider(entity, Collider()) != nullptr;
		case COMPONENT_TYPE_AUDIO_SOURCE: return m_world->AddAudioSource(entity, AudioSourceComponent()) != nullptr;
		case COMPONENT_TYPE_SCRIPT: return m_world->AddScript(entity, ScriptComponent()) != nullptr;
		case COMPONENT_TYPE_LIGHT: return m_world->AddLight(entity, Light()) != nullptr;
		case COMPONENT_TYPE_PREFAB: return m_world->AddPrefab(entity, PrefabComponent()) != nullptr;
		default: return false;
		}
	}

	bool EditorApi::RemoveComponent(_In_ Entity entity, _In_ COMPONENT_TYPE type)
	{
		if (m_world == nullptr)
		{
			return false;
		}
		switch (type)
		{
		case COMPONENT_TYPE_TRANSFORM: return m_world->RemoveTransform(entity);
		case COMPONENT_TYPE_CAMERA: return m_world->RemoveCamera(entity);
		case COMPONENT_TYPE_MESH_RENDERER: return m_world->RemoveMeshRenderer(entity);
		case COMPONENT_TYPE_RIGID_BODY: return m_world->RemoveRigidBody(entity);
		case COMPONENT_TYPE_COLLIDER: return m_world->RemoveCollider(entity);
		case COMPONENT_TYPE_AUDIO_SOURCE: return m_world->RemoveAudioSource(entity);
		case COMPONENT_TYPE_SCRIPT: return m_world->RemoveScript(entity);
		case COMPONENT_TYPE_LIGHT: return m_world->RemoveLight(entity);
		case COMPONENT_TYPE_PREFAB: return m_world->RemovePrefab(entity);
		default: return false;
		}
	}

	bool EditorApi::SetTransform(_In_ Entity entity, _In_ Transform const& value)
	{
		if (m_world == nullptr || !m_world->isAlive(entity))
		{
			return false;
		}
		if (m_world->getTransform(entity) == nullptr)
		{
			return m_world->AddTransform(entity, value) != nullptr;
		}
		*m_world->getTransform(entity) = value;
		return true;
	}

	bool EditorApi::SetComponentJson(_In_ Entity entity, _In_z_ const char* type, _In_ JsonValue const& fields, _Out_opt_ std::string* error)
	{
		COMPONENT_TYPE const kind = ParseType(type);
		if (!AddComponent(entity, kind) && m_world != nullptr)
		{
			if (kind == COMPONENT_TYPE_NONE)
			{
				if (error != nullptr) *error = "unknown component";
				return false;
			}
		}
		if (kind == COMPONENT_TYPE_TRANSFORM)
		{
			Transform value;
			Transform* existing = m_world->getTransform(entity);
			if (existing != nullptr) value = *existing;
			ReadFloat3(fields.Find("position"), &value.Position);
			ReadFloat3(fields.Find("rotation"), &value.Rotation);
			ReadFloat3(fields.Find("scale"), &value.Scale);
			return SetTransform(entity, value);
		}
		if (kind == COMPONENT_TYPE_MESH_RENDERER)
		{
			MeshRenderer* mesh = m_world->getMeshRenderer(entity);
			if (mesh == nullptr) return false;
			JsonValue const* asset = fields.Find("asset");
			if (asset != nullptr) mesh->AssetPath = asset->getString();
			JsonValue const* metallic = fields.Find("metallic");
			if (metallic != nullptr) mesh->Metallic = static_cast<FLOAT>(metallic->getNumber(mesh->Metallic));
			JsonValue const* roughness = fields.Find("roughness");
			if (roughness != nullptr) mesh->Roughness = static_cast<FLOAT>(roughness->getNumber(mesh->Roughness));
			return true;
		}
		if (kind == COMPONENT_TYPE_SCRIPT)
		{
			ScriptComponent* script = m_world->getScript(entity);
			if (script == nullptr) return false;
			JsonValue const* source = fields.Find("source");
			JsonValue const* path = fields.Find("path");
			if (source != nullptr) script->Source = source->getString();
			if (path != nullptr) script->Path = path->getString();
			return true;
		}
		if (kind == COMPONENT_TYPE_LIGHT)
		{
			Light* light = m_world->getLight(entity);
			if (light == nullptr) return false;
			JsonValue const* intensity = fields.Find("intensity");
			if (intensity != nullptr) light->Intensity = static_cast<FLOAT>(intensity->getNumber(light->Intensity));
			ReadFloat3(fields.Find("color"), &light->Color);
			return true;
		}
		return kind != COMPONENT_TYPE_NONE;
	}

	bool EditorApi::ImportGltf(_In_z_ const char* path, _Out_opt_ std::string* error)
	{
		if (path == nullptr || path[0] == '\0')
		{
			if (error != nullptr) *error = "missing gltf path";
			return false;
		}
		std::ifstream file(path, std::ios::binary);
		if (!file)
		{
			if (error != nullptr) *error = "gltf file not found";
			return false;
		}
		m_lastImport = path;
		Entity const entity = CreateEntity("gltf");
		MeshRenderer mesh;
		mesh.AssetPath = path;
		if (m_world != nullptr)
		{
			m_world->AddMeshRenderer(entity, mesh);
			m_world->AddTransform(entity, Transform());
		}
		return true;
	}

	Entity EditorApi::SpawnPrefab(_In_z_ const char* path, _Out_opt_ std::string* error)
	{
		Prefab prefab;
		if (!SceneDocument::LoadPrefab(&prefab, path, error) || m_world == nullptr)
		{
			return kInvalidEntity;
		}
		return m_world->SpawnPrefab(prefab);
	}

	void EditorApi::Play() { m_playing = true; }
	void EditorApi::Stop() { m_playing = false; }
	bool EditorApi::isPlaying() const { return m_playing; }

	bool EditorApi::ExportGame(_In_z_ const char* outputDir, _Out_opt_ std::string* error)
	{
		if (outputDir == nullptr || m_world == nullptr)
		{
			if (error != nullptr) *error = "missing output";
			return false;
		}
		CreateDirectoryA(outputDir, nullptr);
		std::string scenePath = std::string(outputDir) + "\\scene.json";
		if (!SaveScene(scenePath.c_str(), error))
		{
			return false;
		}
		std::string marker = std::string(outputDir) + "\\export.json";
		JsonValue root;
		root.Set("runtime").setBool(true);
		root.Set("editor").setBool(false);
		root.Set("scene").setString("scene.json");
		std::ofstream file(marker, std::ios::binary);
		if (!file)
		{
			if (error != nullptr) *error = "failed to write export manifest";
			return false;
		}
		file << root.Stringify();
		return true;
	}

	bool EditorApi::Screenshot(_In_z_ const char* path, _Out_opt_ std::string* error)
	{
		if (path == nullptr)
		{
			if (error != nullptr) *error = "missing path";
			return false;
		}
		m_lastScreenshot = path;
		std::ofstream file(path, std::ios::binary);
		if (!file)
		{
			if (error != nullptr) *error = "failed to write screenshot";
			return false;
		}
		unsigned char const ppm[] = "P6\n1 1\n255\n";
		file.write(reinterpret_cast<char const*>(ppm), sizeof(ppm) - 1);
		unsigned char const pixel[3] = { 32, 32, 32 };
		file.write(reinterpret_cast<char const*>(pixel), 3);
		return true;
	}

	std::string EditorApi::QueryScene() const
	{
		JsonValue root;
		root.Set("ok").setBool(true);
		root.Set("playing").setBool(m_playing);
		if (m_world != nullptr)
		{
			JsonValue scene;
			std::string error;
			JsonValue::Parse(SceneDocument::ToJson(*m_world).c_str(), &scene, &error);
			root.Set("scene") = scene;
		}
		return root.Stringify();
	}

	const char* EditorApi::getLastImport() const { return m_lastImport.c_str(); }
	const char* EditorApi::getLastScreenshot() const { return m_lastScreenshot.c_str(); }

	struct EditorCommandServer::Impl
	{
		UINT_PTR Listen;
		UINT16 Port;
		bool Listening;
		std::vector<UINT_PTR> Clients;
		std::vector<std::string> Pending;

		Impl()
			: Listen(static_cast<UINT_PTR>(INVALID_SOCKET))
			, Port(0)
			, Listening(false)
		{
		}
	};

	EditorCommandServer::EditorCommandServer()
		: m_impl(new Impl())
		, m_api(nullptr)
	{
	}

	EditorCommandServer::~EditorCommandServer()
	{
		Stop();
		delete m_impl;
		m_impl = nullptr;
	}

	void EditorCommandServer::Attach(_In_ EditorApi* api)
	{
		m_api = api;
	}

	std::string EditorCommandServer::Execute(_In_z_ const char* jsonCommand)
	{
		if (m_api == nullptr)
		{
			return Fail("no editor api");
		}
		JsonValue command;
		std::string error;
		if (!JsonValue::Parse(jsonCommand, &command, &error))
		{
			return Fail(error.c_str());
		}
		JsonValue const* cmd = command.Find("cmd");
		if (cmd == nullptr)
		{
			return Fail("missing cmd");
		}
		const char* name = cmd->getString();
		if (strcmp(name, "new_scene") == 0)
		{
			m_api->NewScene();
			return OkMessage("new scene");
		}
		if (strcmp(name, "open_scene") == 0)
		{
			JsonValue const* path = command.Find("path");
			if (path == nullptr || !m_api->OpenScene(path->getString(), &error)) return Fail(error.empty() ? "open failed" : error.c_str());
			return OkMessage("opened");
		}
		if (strcmp(name, "save_scene") == 0)
		{
			JsonValue const* path = command.Find("path");
			if (path == nullptr || !m_api->SaveScene(path->getString(), &error)) return Fail(error.empty() ? "save failed" : error.c_str());
			return OkMessage("saved");
		}
		if (strcmp(name, "create_entity") == 0)
		{
			JsonValue const* entityName = command.Find("name");
			return OkEntity(m_api->CreateEntity(entityName != nullptr ? entityName->getString() : ""));
		}
		if (strcmp(name, "destroy_entity") == 0)
		{
			return m_api->DestroyEntity(ParseId(command)) ? OkMessage("destroyed") : Fail("destroy failed");
		}
		if (strcmp(name, "add_component") == 0)
		{
			JsonValue const* type = command.Find("type");
			return m_api->AddComponent(ParseId(command), ParseType(type != nullptr ? type->getString() : "")) ? OkMessage("added") : Fail("add failed");
		}
		if (strcmp(name, "remove_component") == 0)
		{
			JsonValue const* type = command.Find("type");
			return m_api->RemoveComponent(ParseId(command), ParseType(type != nullptr ? type->getString() : "")) ? OkMessage("removed") : Fail("remove failed");
		}
		if (strcmp(name, "set_transform") == 0 || strcmp(name, "set_component") == 0)
		{
			JsonValue const* type = command.Find("type");
			const char* typeName = (type != nullptr) ? type->getString() : "transform";
			JsonValue const* fields = command.Find("fields");
			JsonValue const& payload = fields != nullptr ? *fields : command;
			return m_api->SetComponentJson(ParseId(command), typeName, payload, &error) ? OkMessage("updated") : Fail(error.empty() ? "set failed" : error.c_str());
		}
		if (strcmp(name, "import_gltf") == 0)
		{
			JsonValue const* path = command.Find("path");
			if (path == nullptr || !m_api->ImportGltf(path->getString(), &error)) return Fail(error.empty() ? "import failed" : error.c_str());
			return OkMessage(m_api->getLastImport());
		}
		if (strcmp(name, "spawn_prefab") == 0)
		{
			JsonValue const* path = command.Find("path");
			Entity const entity = path != nullptr ? m_api->SpawnPrefab(path->getString(), &error) : kInvalidEntity;
			return entity != kInvalidEntity ? OkEntity(entity) : Fail(error.empty() ? "spawn failed" : error.c_str());
		}
		if (strcmp(name, "play") == 0)
		{
			m_api->Play();
			return OkMessage("playing");
		}
		if (strcmp(name, "stop") == 0)
		{
			m_api->Stop();
			return OkMessage("stopped");
		}
		if (strcmp(name, "export_game") == 0)
		{
			JsonValue const* path = command.Find("path");
			if (path == nullptr || !m_api->ExportGame(path->getString(), &error)) return Fail(error.empty() ? "export failed" : error.c_str());
			return OkMessage("exported");
		}
		if (strcmp(name, "screenshot") == 0)
		{
			JsonValue const* path = command.Find("path");
			if (path == nullptr || !m_api->Screenshot(path->getString(), &error)) return Fail(error.empty() ? "screenshot failed" : error.c_str());
			return OkMessage("screenshot");
		}
		if (strcmp(name, "query_scene") == 0)
		{
			return m_api->QueryScene();
		}
		return Fail("unknown command");
	}

	bool EditorCommandServer::ExecuteFile(_In_z_ const char* path, _Out_ std::string* transcript)
	{
		std::ifstream file(path);
		if (!file)
		{
			return false;
		}
		std::string line;
		if (transcript != nullptr)
		{
			transcript->clear();
		}
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
			{
				continue;
			}
			std::string response = Execute(line.c_str());
			if (transcript != nullptr)
			{
				*transcript += response;
				*transcript += "\n";
			}
		}
		return true;
	}

	bool EditorCommandServer::Start(_In_ UINT16 port)
	{
		Stop();
		if (port != 0 && port < 1024)
		{
			return false;
		}
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			return false;
		}
		SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listenSocket == INVALID_SOCKET)
		{
			return false;
		}
		u_long mode = 1;
		ioctlsocket(listenSocket, FIONBIO, &mode);
		BOOL reuse = TRUE;
		setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char const*>(&reuse), sizeof(reuse));
		sockaddr_in address;
		ZeroMemory(&address, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		address.sin_port = htons(port);
		if (bind(listenSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR || listen(listenSocket, 8) == SOCKET_ERROR)
		{
			closesocket(listenSocket);
			return false;
		}
		sockaddr_in bound;
		int length = sizeof(bound);
		getsockname(listenSocket, reinterpret_cast<sockaddr*>(&bound), &length);
		m_impl->Listen = static_cast<UINT_PTR>(listenSocket);
		m_impl->Port = ntohs(bound.sin_port);
		m_impl->Listening = true;
		return true;
	}

	void EditorCommandServer::Stop()
	{
		if (m_impl == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < m_impl->Clients.size(); ++i)
		{
			closesocket(static_cast<SOCKET>(m_impl->Clients[i]));
		}
		m_impl->Clients.clear();
		m_impl->Pending.clear();
		if (m_impl->Listen != static_cast<UINT_PTR>(INVALID_SOCKET))
		{
			closesocket(static_cast<SOCKET>(m_impl->Listen));
			m_impl->Listen = static_cast<UINT_PTR>(INVALID_SOCKET);
		}
		m_impl->Listening = false;
		m_impl->Port = 0;
	}

	void EditorCommandServer::Poll()
	{
		if (m_impl == nullptr || !m_impl->Listening)
		{
			return;
		}
		for (;;)
		{
			SOCKET accepted = accept(static_cast<SOCKET>(m_impl->Listen), nullptr, nullptr);
			if (accepted == INVALID_SOCKET)
			{
				break;
			}
			u_long mode = 1;
			ioctlsocket(accepted, FIONBIO, &mode);
			m_impl->Clients.push_back(static_cast<UINT_PTR>(accepted));
			m_impl->Pending.push_back(std::string());
		}
		for (size_t i = 0; i < m_impl->Clients.size();)
		{
			char temp[1024];
			int const received = recv(static_cast<SOCKET>(m_impl->Clients[i]), temp, static_cast<int>(sizeof(temp)), 0);
			if (received > 0)
			{
				m_impl->Pending[i].append(temp, temp + received);
			}
			else if (received == 0)
			{
				closesocket(static_cast<SOCKET>(m_impl->Clients[i]));
				m_impl->Clients.erase(m_impl->Clients.begin() + static_cast<std::ptrdiff_t>(i));
				m_impl->Pending.erase(m_impl->Pending.begin() + static_cast<std::ptrdiff_t>(i));
				continue;
			}
			size_t newline = m_impl->Pending[i].find('\n');
			if (newline != std::string::npos)
			{
				std::string line = m_impl->Pending[i].substr(0, newline);
				m_impl->Pending[i].erase(0, newline + 1);
				if (!line.empty() && line.back() == '\r')
				{
					line.pop_back();
				}
				std::string response = Execute(line.c_str());
				response.push_back('\n');
				send(static_cast<SOCKET>(m_impl->Clients[i]), response.data(), static_cast<int>(response.size()), 0);
			}
			++i;
		}
	}

	bool EditorCommandServer::isListening() const
	{
		return m_impl != nullptr && m_impl->Listening;
	}

	UINT16 EditorCommandServer::getPort() const
	{
		return m_impl != nullptr ? m_impl->Port : 0;
	}
}
