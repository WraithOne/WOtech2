////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Scene.cpp
///
///			Description:
///			Save and load worlds and prefabs as versioned JSON.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Scene.hpp"
#include "WO_Json.hpp"

#include <fstream>
#include <sstream>

namespace WOtech
{
	namespace
	{
		void WriteFloat3(_Inout_ JsonValue* value, _In_ DirectX::XMFLOAT3 const& v)
		{
			value->Add().setNumber(v.x);
			value->Add().setNumber(v.y);
			value->Add().setNumber(v.z);
		}

		bool ReadFloat3(_In_ JsonValue const* value, _Out_ DirectX::XMFLOAT3* out)
		{
			if (value == nullptr || out == nullptr || !value->isArray() || value->getCount() < 3)
			{
				return false;
			}
			out->x = static_cast<FLOAT>(value->getAt(0)->getNumber(0.0));
			out->y = static_cast<FLOAT>(value->getAt(1)->getNumber(0.0));
			out->z = static_cast<FLOAT>(value->getAt(2)->getNumber(0.0));
			return true;
		}

		void WriteColor4(_Inout_ JsonValue* value, _In_reads_(4) FLOAT const* color)
		{
			for (int i = 0; i < 4; ++i)
			{
				value->Add().setNumber(color[i]);
			}
		}

		const char* BodyTypeName(_In_ BODY_TYPE type)
		{
			switch (type)
			{
			case BODY_TYPE_STATIC: return "static";
			case BODY_TYPE_KINEMATIC: return "kinematic";
			default: return "dynamic";
			}
		}

		BODY_TYPE ParseBodyType(_In_z_ const char* name)
		{
			if (name != nullptr && strcmp(name, "static") == 0) return BODY_TYPE_STATIC;
			if (name != nullptr && strcmp(name, "kinematic") == 0) return BODY_TYPE_KINEMATIC;
			return BODY_TYPE_DYNAMIC;
		}

		const char* ShapeName(_In_ COLLIDER_SHAPE shape)
		{
			switch (shape)
			{
			case COLLIDER_SHAPE_SPHERE: return "sphere";
			case COLLIDER_SHAPE_CAPSULE: return "capsule";
			default: return "box";
			}
		}

		COLLIDER_SHAPE ParseShape(_In_z_ const char* name)
		{
			if (name != nullptr && strcmp(name, "sphere") == 0) return COLLIDER_SHAPE_SPHERE;
			if (name != nullptr && strcmp(name, "capsule") == 0) return COLLIDER_SHAPE_CAPSULE;
			return COLLIDER_SHAPE_BOX;
		}

		const char* LightName(_In_ LIGHT_KIND kind)
		{
			switch (kind)
			{
			case LIGHT_KIND_POINT: return "point";
			case LIGHT_KIND_SPOT: return "spot";
			default: return "directional";
			}
		}

		LIGHT_KIND ParseLight(_In_z_ const char* name)
		{
			if (name != nullptr && strcmp(name, "point") == 0) return LIGHT_KIND_POINT;
			if (name != nullptr && strcmp(name, "spot") == 0) return LIGHT_KIND_SPOT;
			return LIGHT_KIND_DIRECTIONAL;
		}

		void WriteEntity(_Inout_ JsonValue* item, _In_ World const& world, _In_ Entity entity)
		{
			char id[32];
			sprintf_s(id, "%llu", static_cast<unsigned long long>(entity));
			item->Set("id").setString(id);
			item->Set("name").setString(world.getName(entity));

			Transform const* transform = world.getTransform(entity);
			if (transform != nullptr)
			{
				JsonValue& node = item->Set("transform");
				WriteFloat3(&node.Set("position"), transform->Position);
				WriteFloat3(&node.Set("rotation"), transform->Rotation);
				WriteFloat3(&node.Set("scale"), transform->Scale);
			}
			MeshRenderer const* mesh = world.getMeshRenderer(entity);
			if (mesh != nullptr)
			{
				JsonValue& node = item->Set("meshRenderer");
				node.Set("asset").setString(mesh->AssetPath.c_str());
				WriteColor4(&node.Set("baseColor"), mesh->BaseColor);
				node.Set("metallic").setNumber(mesh->Metallic);
				node.Set("roughness").setNumber(mesh->Roughness);
				node.Set("castShadow").setBool(mesh->CastShadow);
			}
			RigidBody const* body = world.getRigidBody(entity);
			if (body != nullptr)
			{
				JsonValue& node = item->Set("rigidBody");
				node.Set("type").setString(BodyTypeName(body->Type));
				node.Set("mass").setNumber(body->Mass);
				node.Set("useGravity").setBool(body->UseGravity);
				WriteFloat3(&node.Set("linearVelocity"), body->LinearVelocity);
			}
			Collider const* collider = world.getCollider(entity);
			if (collider != nullptr)
			{
				JsonValue& node = item->Set("collider");
				node.Set("shape").setString(ShapeName(collider->Shape));
				WriteFloat3(&node.Set("halfExtents"), collider->HalfExtents);
				node.Set("radius").setNumber(collider->Radius);
				node.Set("height").setNumber(collider->Height);
				node.Set("sensor").setBool(collider->IsSensor);
			}
			AudioSourceComponent const* audio = world.getAudioSource(entity);
			if (audio != nullptr)
			{
				JsonValue& node = item->Set("audioSource");
				node.Set("file").setString(audio->FileName.c_str());
				node.Set("volume").setNumber(audio->Volume);
				node.Set("playOnStart").setBool(audio->PlayOnStart);
				node.Set("loop").setBool(audio->Loop);
			}
			ScriptComponent const* script = world.getScript(entity);
			if (script != nullptr)
			{
				JsonValue& node = item->Set("script");
				node.Set("path").setString(script->Path.c_str());
				node.Set("source").setString(script->Source.c_str());
				node.Set("enabled").setBool(script->Enabled);
			}
			Light const* light = world.getLight(entity);
			if (light != nullptr)
			{
				JsonValue& node = item->Set("light");
				node.Set("kind").setString(LightName(light->Kind));
				WriteFloat3(&node.Set("color"), light->Color);
				node.Set("intensity").setNumber(light->Intensity);
				node.Set("range").setNumber(light->Range);
				node.Set("spotAngle").setNumber(light->SpotAngle);
				node.Set("castShadows").setBool(light->CastShadows);
			}
			CameraComponent const* camera = world.getCamera(entity);
			if (camera != nullptr)
			{
				JsonValue& node = item->Set("camera");
				node.Set("primary").setBool(camera->Primary);
				node.Set("fov").setNumber(camera->FieldOfViewDegrees);
			}
			PrefabComponent const* prefab = world.getPrefab(entity);
			if (prefab != nullptr)
			{
				item->Set("prefab").Set("path").setString(prefab->PrefabPath.c_str());
			}
		}

		void ReadEntity(_In_ JsonValue const& item, _Inout_ World* world)
		{
			JsonValue const* name = item.Find("name");
			Entity const entity = world->CreateEntity(name != nullptr ? name->getString() : "");
			JsonValue const* transform = item.Find("transform");
			if (transform != nullptr)
			{
				Transform value;
				ReadFloat3(transform->Find("position"), &value.Position);
				ReadFloat3(transform->Find("rotation"), &value.Rotation);
				ReadFloat3(transform->Find("scale"), &value.Scale);
				world->AddTransform(entity, value);
			}
			JsonValue const* mesh = item.Find("meshRenderer");
			if (mesh != nullptr)
			{
				MeshRenderer value;
				JsonValue const* asset = mesh->Find("asset");
				if (asset != nullptr)
				{
					value.AssetPath = asset->getString();
				}
				JsonValue const* color = mesh->Find("baseColor");
				if (color != nullptr && color->isArray())
				{
					for (UINT32 i = 0; i < 4 && i < color->getCount(); ++i)
					{
						value.BaseColor[i] = static_cast<FLOAT>(color->getAt(i)->getNumber(1.0));
					}
				}
				value.Metallic = static_cast<FLOAT>(mesh->Find("metallic") != nullptr ? mesh->Find("metallic")->getNumber(0.0) : 0.0);
				value.Roughness = static_cast<FLOAT>(mesh->Find("roughness") != nullptr ? mesh->Find("roughness")->getNumber(0.5) : 0.5);
				value.CastShadow = mesh->Find("castShadow") != nullptr ? mesh->Find("castShadow")->getBool(true) : true;
				world->AddMeshRenderer(entity, value);
			}
			JsonValue const* body = item.Find("rigidBody");
			if (body != nullptr)
			{
				RigidBody value;
				JsonValue const* type = body->Find("type");
				value.Type = ParseBodyType(type != nullptr ? type->getString() : "dynamic");
				value.Mass = static_cast<FLOAT>(body->Find("mass") != nullptr ? body->Find("mass")->getNumber(1.0) : 1.0);
				value.UseGravity = body->Find("useGravity") != nullptr ? body->Find("useGravity")->getBool(true) : true;
				JsonValue const* velocity = body->Find("linearVelocity");
				if (velocity != nullptr)
				{
					ReadFloat3(velocity, &value.LinearVelocity);
				}
				world->AddRigidBody(entity, value);
			}
			JsonValue const* collider = item.Find("collider");
			if (collider != nullptr)
			{
				Collider value;
				JsonValue const* shape = collider->Find("shape");
				value.Shape = ParseShape(shape != nullptr ? shape->getString() : "box");
				ReadFloat3(collider->Find("halfExtents"), &value.HalfExtents);
				value.Radius = static_cast<FLOAT>(collider->Find("radius") != nullptr ? collider->Find("radius")->getNumber(0.5) : 0.5);
				value.Height = static_cast<FLOAT>(collider->Find("height") != nullptr ? collider->Find("height")->getNumber(1.0) : 1.0);
				value.IsSensor = collider->Find("sensor") != nullptr ? collider->Find("sensor")->getBool(false) : false;
				world->AddCollider(entity, value);
			}
			JsonValue const* audio = item.Find("audioSource");
			if (audio != nullptr)
			{
				AudioSourceComponent value;
				JsonValue const* file = audio->Find("file");
				if (file != nullptr)
				{
					value.FileName = file->getString();
				}
				value.Volume = static_cast<FLOAT>(audio->Find("volume") != nullptr ? audio->Find("volume")->getNumber(1.0) : 1.0);
				value.PlayOnStart = audio->Find("playOnStart") != nullptr ? audio->Find("playOnStart")->getBool(false) : false;
				value.Loop = audio->Find("loop") != nullptr ? audio->Find("loop")->getBool(false) : false;
				world->AddAudioSource(entity, value);
			}
			JsonValue const* script = item.Find("script");
			if (script != nullptr)
			{
				ScriptComponent value;
				JsonValue const* path = script->Find("path");
				JsonValue const* source = script->Find("source");
				if (path != nullptr) value.Path = path->getString();
				if (source != nullptr) value.Source = source->getString();
				value.Enabled = script->Find("enabled") != nullptr ? script->Find("enabled")->getBool(true) : true;
				world->AddScript(entity, value);
			}
			JsonValue const* light = item.Find("light");
			if (light != nullptr)
			{
				Light value;
				JsonValue const* kind = light->Find("kind");
				value.Kind = ParseLight(kind != nullptr ? kind->getString() : "directional");
				JsonValue const* color = light->Find("color");
				if (color != nullptr && color->getCount() >= 3)
				{
					value.Color.x = static_cast<FLOAT>(color->getAt(0)->getNumber(1.0));
					value.Color.y = static_cast<FLOAT>(color->getAt(1)->getNumber(1.0));
					value.Color.z = static_cast<FLOAT>(color->getAt(2)->getNumber(1.0));
				}
				value.Intensity = static_cast<FLOAT>(light->Find("intensity") != nullptr ? light->Find("intensity")->getNumber(1.0) : 1.0);
				value.Range = static_cast<FLOAT>(light->Find("range") != nullptr ? light->Find("range")->getNumber(10.0) : 10.0);
				value.SpotAngle = static_cast<FLOAT>(light->Find("spotAngle") != nullptr ? light->Find("spotAngle")->getNumber(0.5) : 0.5);
				value.CastShadows = light->Find("castShadows") != nullptr ? light->Find("castShadows")->getBool(true) : true;
				world->AddLight(entity, value);
			}
			JsonValue const* camera = item.Find("camera");
			if (camera != nullptr)
			{
				CameraComponent value;
				value.Primary = camera->Find("primary") != nullptr ? camera->Find("primary")->getBool(false) : false;
				value.FieldOfViewDegrees = static_cast<FLOAT>(camera->Find("fov") != nullptr ? camera->Find("fov")->getNumber(60.0) : 60.0);
				value.View.SetProjParams(value.FieldOfViewDegrees * 0.0174532925f, 16.0f / 9.0f, 0.1f, 1000.0f);
				world->AddCamera(entity, value);
			}
			JsonValue const* prefab = item.Find("prefab");
			if (prefab != nullptr)
			{
				PrefabComponent value;
				JsonValue const* path = prefab->Find("path");
				if (path != nullptr)
				{
					value.PrefabPath = path->getString();
				}
				world->AddPrefab(entity, value);
			}
		}

		bool ReadFile(_In_z_ const char* path, _Out_ std::string* text, _Out_opt_ std::string* error)
		{
			std::ifstream file(path, std::ios::binary);
			if (!file)
			{
				if (error != nullptr)
				{
					*error = "failed to open file";
				}
				return false;
			}
			std::ostringstream buffer;
			buffer << file.rdbuf();
			*text = buffer.str();
			return true;
		}

		bool WriteFile(_In_z_ const char* path, _In_ std::string const& text, _Out_opt_ std::string* error)
		{
			std::ofstream file(path, std::ios::binary);
			if (!file)
			{
				if (error != nullptr)
				{
					*error = "failed to write file";
				}
				return false;
			}
			file << text;
			return true;
		}
	}

	std::string SceneDocument::ToJson(_In_ World const& world)
	{
		JsonValue root;
		root.Set("format").setString("wotech2-scene");
		root.Set("version").setNumber(kSceneVersion);
		JsonValue& entities = root.Set("entities");
		std::vector<Entity> alive;
		world.GetAliveEntities(&alive);
		for (size_t i = 0; i < alive.size(); ++i)
		{
			WriteEntity(&entities.Add(), world, alive[i]);
		}
		return root.Stringify();
	}

	bool SceneDocument::FromJson(_Inout_ World* world, _In_z_ const char* json, _Out_opt_ std::string* error)
	{
		if (world == nullptr)
		{
			if (error != nullptr) *error = "null world";
			return false;
		}
		JsonValue root;
		if (!JsonValue::Parse(json, &root, error))
		{
			return false;
		}
		JsonValue const* format = root.Find("format");
		if (format == nullptr || strcmp(format->getString(), "wotech2-scene") != 0)
		{
			if (error != nullptr) *error = "unsupported scene format";
			return false;
		}
		JsonValue const* version = root.Find("version");
		if (version == nullptr || static_cast<INT>(version->getNumber(0.0)) != kSceneVersion)
		{
			if (error != nullptr) *error = "unsupported scene version";
			return false;
		}
		world->Clear();
		JsonValue const* entities = root.Find("entities");
		if (entities != nullptr && entities->isArray())
		{
			for (UINT32 i = 0; i < entities->getCount(); ++i)
			{
				JsonValue const* item = entities->getAt(i);
				if (item != nullptr)
				{
					ReadEntity(*item, world);
				}
			}
		}
		return true;
	}

	bool SceneDocument::Save(_In_ World const& world, _In_z_ const char* path, _Out_opt_ std::string* error)
	{
		return WriteFile(path, ToJson(world), error);
	}

	bool SceneDocument::Load(_Inout_ World* world, _In_z_ const char* path, _Out_opt_ std::string* error)
	{
		std::string text;
		if (!ReadFile(path, &text, error))
		{
			return false;
		}
		return FromJson(world, text.c_str(), error);
	}

	std::string SceneDocument::PrefabToJson(_In_ Prefab const& prefab)
	{
		World world;
		world.SpawnPrefab(prefab);
		std::string scene = ToJson(world);
		JsonValue root;
		std::string error;
		JsonValue::Parse(scene.c_str(), &root, &error);
		root.Set("format").setString("wotech2-prefab");
		return root.Stringify();
	}

	bool SceneDocument::PrefabFromJson(_Out_ Prefab* prefab, _In_z_ const char* json, _Out_opt_ std::string* error)
	{
		if (prefab == nullptr)
		{
			return false;
		}
		JsonValue root;
		if (!JsonValue::Parse(json, &root, error))
		{
			return false;
		}
		root.Set("format").setString("wotech2-scene");
		std::string rewritten = root.Stringify();
		World world;
		if (!FromJson(&world, rewritten.c_str(), error))
		{
			return false;
		}
		std::vector<Entity> alive;
		world.GetAliveEntities(&alive);
		if (alive.empty())
		{
			if (error != nullptr) *error = "prefab has no entity";
			return false;
		}
		Entity const entity = alive[0];
		*prefab = Prefab();
		prefab->setName(world.getName(entity));
		if (world.getTransform(entity) != nullptr) prefab->setTransform(*world.getTransform(entity));
		if (world.getCamera(entity) != nullptr) prefab->setCamera(*world.getCamera(entity));
		if (world.getMeshRenderer(entity) != nullptr) prefab->setMeshRenderer(*world.getMeshRenderer(entity));
		if (world.getRigidBody(entity) != nullptr) prefab->setRigidBody(*world.getRigidBody(entity));
		if (world.getCollider(entity) != nullptr) prefab->setCollider(*world.getCollider(entity));
		if (world.getAudioSource(entity) != nullptr) prefab->setAudioSource(*world.getAudioSource(entity));
		if (world.getScript(entity) != nullptr) prefab->setScript(*world.getScript(entity));
		if (world.getLight(entity) != nullptr) prefab->setLight(*world.getLight(entity));
		if (world.getPrefab(entity) != nullptr) prefab->setPrefabPath(world.getPrefab(entity)->PrefabPath.c_str());
		return true;
	}

	bool SceneDocument::SavePrefab(_In_ Prefab const& prefab, _In_z_ const char* path, _Out_opt_ std::string* error)
	{
		return WriteFile(path, PrefabToJson(prefab), error);
	}

	bool SceneDocument::LoadPrefab(_Out_ Prefab* prefab, _In_z_ const char* path, _Out_opt_ std::string* error)
	{
		std::string text;
		if (!ReadFile(path, &text, error))
		{
			return false;
		}
		return PrefabFromJson(prefab, text.c_str(), error);
	}
}
