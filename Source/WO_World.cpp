////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_World.cpp
///
///			Description:
///			Entity create/destroy, component access, and prefab spawn.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_World.hpp"

namespace WOtech
{
	Prefab::Prefab()
		: m_hasTransform(false)
		, m_hasCamera(false)
		, m_hasMeshRenderer(false)
		, m_hasRigidBody(false)
		, m_hasCollider(false)
		, m_hasAudio(false)
		, m_hasScript(false)
		, m_hasLight(false)
	{
	}

	void Prefab::setName(_In_z_ const char* name)
	{
		m_name = (name != nullptr) ? name : "";
	}

	const char* Prefab::getName() const
	{
		return m_name.c_str();
	}

	void Prefab::setTransform(_In_ Transform const& value)
	{
		m_transform = value;
		m_hasTransform = true;
	}

	Transform const* Prefab::getTransform() const
	{
		return m_hasTransform ? &m_transform : nullptr;
	}

	void Prefab::ClearTransform()
	{
		m_hasTransform = false;
		m_transform = Transform();
	}

	void Prefab::setCamera(_In_ CameraComponent const& value)
	{
		m_camera = value;
		m_hasCamera = true;
	}

	CameraComponent const* Prefab::getCamera() const
	{
		return m_hasCamera ? &m_camera : nullptr;
	}

	void Prefab::setMeshRenderer(_In_ MeshRenderer const& value)
	{
		m_meshRenderer = value;
		m_hasMeshRenderer = true;
	}

	MeshRenderer const* Prefab::getMeshRenderer() const
	{
		return m_hasMeshRenderer ? &m_meshRenderer : nullptr;
	}

	void Prefab::setRigidBody(_In_ RigidBody const& value)
	{
		m_rigidBody = value;
		m_hasRigidBody = true;
	}

	RigidBody const* Prefab::getRigidBody() const
	{
		return m_hasRigidBody ? &m_rigidBody : nullptr;
	}

	void Prefab::setCollider(_In_ Collider const& value)
	{
		m_collider = value;
		m_hasCollider = true;
	}

	Collider const* Prefab::getCollider() const
	{
		return m_hasCollider ? &m_collider : nullptr;
	}

	void Prefab::setAudioSource(_In_ AudioSourceComponent const& value)
	{
		m_audio = value;
		m_hasAudio = true;
	}

	AudioSourceComponent const* Prefab::getAudioSource() const
	{
		return m_hasAudio ? &m_audio : nullptr;
	}

	void Prefab::setScript(_In_ ScriptComponent const& value)
	{
		m_script = value;
		m_hasScript = true;
	}

	ScriptComponent const* Prefab::getScript() const
	{
		return m_hasScript ? &m_script : nullptr;
	}

	void Prefab::setLight(_In_ Light const& value)
	{
		m_light = value;
		m_hasLight = true;
	}

	Light const* Prefab::getLight() const
	{
		return m_hasLight ? &m_light : nullptr;
	}

	void Prefab::setPrefabPath(_In_z_ const char* path)
	{
		m_prefabPath = (path != nullptr) ? path : "";
	}

	const char* Prefab::getPrefabPath() const
	{
		return m_prefabPath.c_str();
	}

	World::World()
		: m_aliveCount(0)
	{
	}

	World::~World()
	{
		Clear();
	}

	bool World::Valid(_In_ Entity entity) const
	{
		if (entity == kInvalidEntity)
		{
			return false;
		}
		UINT32 const index = EntityIndex(entity);
		if (index >= m_records.size())
		{
			return false;
		}
		Record const& record = m_records[index];
		return record.Alive && record.Generation == EntityGeneration(entity);
	}

	Entity World::CreateEntity()
	{
		return CreateEntity("");
	}

	Entity World::CreateEntity(_In_z_ const char* name)
	{
		UINT32 index = 0;
		if (!m_free.empty())
		{
			index = m_free.back();
			m_free.pop_back();
		}
		else
		{
			index = static_cast<UINT32>(m_records.size());
			m_records.push_back(Record());
		}

		Record& record = m_records[index];
		if (record.Generation == 0)
		{
			record.Generation = 1;
		}
		record.Alive = true;
		record.Name = (name != nullptr) ? name : "";
		m_aliveCount += 1;
		return MakeEntity(index, record.Generation);
	}

	void World::DestroyEntity(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return;
		}

		UINT32 const index = EntityIndex(entity);
		Record& record = m_records[index];
		record.Alive = false;
		record.Name.clear();
		record.Generation += 1;
		if (record.Generation == 0)
		{
			record.Generation = 1;
		}

		m_transforms.Destroy(entity);
		m_cameras.Destroy(entity);
		m_meshRenderers.Destroy(entity);
		m_rigidBodies.Destroy(entity);
		m_colliders.Destroy(entity);
		m_audioSources.Destroy(entity);
		m_scripts.Destroy(entity);
		m_lights.Destroy(entity);
		m_prefabs.Destroy(entity);

		m_free.push_back(index);
		if (m_aliveCount > 0)
		{
			m_aliveCount -= 1;
		}
	}

	bool World::isAlive(_In_ Entity entity) const
	{
		return Valid(entity);
	}

	void World::setName(_In_ Entity entity, _In_z_ const char* name)
	{
		if (!Valid(entity))
		{
			return;
		}
		m_records[EntityIndex(entity)].Name = (name != nullptr) ? name : "";
	}

	const char* World::getName(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return "";
		}
		return m_records[EntityIndex(entity)].Name.c_str();
	}

	UINT32 World::getAliveCount() const
	{
		return m_aliveCount;
	}

	void World::GetAliveEntities(_Out_ std::vector<Entity>* entities) const
	{
		if (entities == nullptr)
		{
			return;
		}
		entities->clear();
		entities->reserve(m_aliveCount);
		for (UINT32 index = 0; index < static_cast<UINT32>(m_records.size()); ++index)
		{
			Record const& record = m_records[index];
			if (record.Alive)
			{
				entities->push_back(MakeEntity(index, record.Generation));
			}
		}
	}

	Transform* World::AddTransform(_In_ Entity entity, _In_ Transform const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_transforms.Add(entity, value);
	}

	Transform* World::getTransform(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_transforms.get(entity);
	}

	Transform const* World::getTransform(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_transforms.get(entity);
	}

	bool World::RemoveTransform(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_transforms.Remove(entity);
	}

	CameraComponent* World::AddCamera(_In_ Entity entity, _In_ CameraComponent const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_cameras.Add(entity, value);
	}

	CameraComponent* World::getCamera(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_cameras.get(entity);
	}

	bool World::RemoveCamera(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_cameras.Remove(entity);
	}

	MeshRenderer* World::AddMeshRenderer(_In_ Entity entity, _In_ MeshRenderer const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_meshRenderers.Add(entity, value);
	}

	MeshRenderer* World::getMeshRenderer(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_meshRenderers.get(entity);
	}

	bool World::RemoveMeshRenderer(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_meshRenderers.Remove(entity);
	}

	RigidBody* World::AddRigidBody(_In_ Entity entity, _In_ RigidBody const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_rigidBodies.Add(entity, value);
	}

	RigidBody* World::getRigidBody(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_rigidBodies.get(entity);
	}

	bool World::RemoveRigidBody(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_rigidBodies.Remove(entity);
	}

	Collider* World::AddCollider(_In_ Entity entity, _In_ Collider const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_colliders.Add(entity, value);
	}

	Collider* World::getCollider(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_colliders.get(entity);
	}

	bool World::RemoveCollider(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_colliders.Remove(entity);
	}

	AudioSourceComponent* World::AddAudioSource(_In_ Entity entity, _In_ AudioSourceComponent const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_audioSources.Add(entity, value);
	}

	AudioSourceComponent* World::getAudioSource(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_audioSources.get(entity);
	}

	bool World::RemoveAudioSource(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_audioSources.Remove(entity);
	}

	ScriptComponent* World::AddScript(_In_ Entity entity, _In_ ScriptComponent const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_scripts.Add(entity, value);
	}

	ScriptComponent* World::getScript(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_scripts.get(entity);
	}

	bool World::RemoveScript(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_scripts.Remove(entity);
	}

	Light* World::AddLight(_In_ Entity entity, _In_ Light const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_lights.Add(entity, value);
	}

	Light* World::getLight(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_lights.get(entity);
	}

	bool World::RemoveLight(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_lights.Remove(entity);
	}

	PrefabComponent* World::AddPrefab(_In_ Entity entity, _In_ PrefabComponent const& value)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_prefabs.Add(entity, value);
	}

	PrefabComponent* World::getPrefab(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_prefabs.get(entity);
	}

	bool World::RemovePrefab(_In_ Entity entity)
	{
		if (!Valid(entity))
		{
			return false;
		}
		return m_prefabs.Remove(entity);
	}

	CameraComponent const* World::getCamera(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_cameras.get(entity);
	}

	MeshRenderer const* World::getMeshRenderer(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_meshRenderers.get(entity);
	}

	RigidBody const* World::getRigidBody(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_rigidBodies.get(entity);
	}

	Collider const* World::getCollider(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_colliders.get(entity);
	}

	AudioSourceComponent const* World::getAudioSource(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_audioSources.get(entity);
	}

	ScriptComponent const* World::getScript(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_scripts.get(entity);
	}

	Light const* World::getLight(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_lights.get(entity);
	}

	PrefabComponent const* World::getPrefab(_In_ Entity entity) const
	{
		if (!Valid(entity))
		{
			return nullptr;
		}
		return m_prefabs.get(entity);
	}

	Entity World::SpawnPrefab(_In_ Prefab const& prefab)
	{
		Entity const entity = CreateEntity(prefab.getName());
		Transform const* transform = prefab.getTransform();
		if (transform != nullptr)
		{
			AddTransform(entity, *transform);
		}
		CameraComponent const* camera = prefab.getCamera();
		if (camera != nullptr)
		{
			AddCamera(entity, *camera);
		}
		MeshRenderer const* mesh = prefab.getMeshRenderer();
		if (mesh != nullptr)
		{
			AddMeshRenderer(entity, *mesh);
		}
		RigidBody const* body = prefab.getRigidBody();
		if (body != nullptr)
		{
			RigidBody spawned = *body;
			spawned.PhysicsHandle = 0;
			AddRigidBody(entity, spawned);
		}
		Collider const* collider = prefab.getCollider();
		if (collider != nullptr)
		{
			AddCollider(entity, *collider);
		}
		AudioSourceComponent const* audio = prefab.getAudioSource();
		if (audio != nullptr)
		{
			AudioSourceComponent spawned = *audio;
			spawned.Playing = false;
			AddAudioSource(entity, spawned);
		}
		ScriptComponent const* script = prefab.getScript();
		if (script != nullptr)
		{
			ScriptComponent spawned = *script;
			spawned.LastError.clear();
			AddScript(entity, spawned);
		}
		Light const* light = prefab.getLight();
		if (light != nullptr)
		{
			AddLight(entity, *light);
		}
		if (prefab.getPrefabPath()[0] != '\0')
		{
			PrefabComponent reference;
			reference.PrefabPath = prefab.getPrefabPath();
			AddPrefab(entity, reference);
		}
		return entity;
	}

	void World::Clear()
	{
		std::vector<Entity> alive;
		GetAliveEntities(&alive);
		for (size_t i = 0; i < alive.size(); ++i)
		{
			DestroyEntity(alive[i]);
		}
	}
}
