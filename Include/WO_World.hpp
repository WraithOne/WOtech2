////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_World.hpp
///
///			Description:
///			Entity world. Create and destroy entities, store components,
///			and spawn in-memory prefabs.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_WORLD_H
#define WO_WORLD_H

//////////////
// INCLUDES //
//////////////
#include "WO_Entity.hpp"
#include "WO_Components.hpp"

#include <vector>

namespace WOtech
{
	class Prefab
	{
	public:
		Prefab();

		void setName(_In_z_ const char* name);
		const char* getName() const;

		void setTransform(_In_ Transform const& value);
		Transform const* getTransform() const;
		void ClearTransform();

		void setCamera(_In_ CameraComponent const& value);
		CameraComponent const* getCamera() const;

		void setMeshRenderer(_In_ MeshRenderer const& value);
		MeshRenderer const* getMeshRenderer() const;

		void setRigidBody(_In_ RigidBody const& value);
		RigidBody const* getRigidBody() const;

		void setCollider(_In_ Collider const& value);
		Collider const* getCollider() const;

		void setAudioSource(_In_ AudioSourceComponent const& value);
		AudioSourceComponent const* getAudioSource() const;

		void setScript(_In_ ScriptComponent const& value);
		ScriptComponent const* getScript() const;

		void setLight(_In_ Light const& value);
		Light const* getLight() const;

		void setPrefabPath(_In_z_ const char* path);
		const char* getPrefabPath() const;

	private:
		std::string				m_name;
		bool					m_hasTransform;
		Transform				m_transform;
		bool					m_hasCamera;
		CameraComponent			m_camera;
		bool					m_hasMeshRenderer;
		MeshRenderer			m_meshRenderer;
		bool					m_hasRigidBody;
		RigidBody				m_rigidBody;
		bool					m_hasCollider;
		Collider				m_collider;
		bool					m_hasAudio;
		AudioSourceComponent	m_audio;
		bool					m_hasScript;
		ScriptComponent			m_script;
		bool					m_hasLight;
		Light					m_light;
		std::string				m_prefabPath;
	};

	template<typename TComponent>
	class ComponentStorage
	{
	public:
		ComponentStorage()
			: m_count(0)
		{
		}

		TComponent* Add(_In_ Entity entity, _In_ TComponent const& value)
		{
			UINT32 const index = EntityIndex(entity);
			Ensure(index);
			Slot& slot = m_slots[index];
			if (!slot.Occupied)
			{
				m_count += 1;
			}
			slot.Generation = EntityGeneration(entity);
			slot.Occupied = true;
			slot.Value = value;
			return &slot.Value;
		}

		TComponent* get(_In_ Entity entity)
		{
			return const_cast<TComponent*>(static_cast<ComponentStorage const*>(this)->get(entity));
		}

		TComponent const* get(_In_ Entity entity) const
		{
			UINT32 const index = EntityIndex(entity);
			if (index >= m_slots.size())
			{
				return nullptr;
			}
			Slot const& slot = m_slots[index];
			if (!slot.Occupied || slot.Generation != EntityGeneration(entity))
			{
				return nullptr;
			}
			return &slot.Value;
		}

		bool Remove(_In_ Entity entity)
		{
			UINT32 const index = EntityIndex(entity);
			if (index >= m_slots.size())
			{
				return false;
			}
			Slot& slot = m_slots[index];
			if (!slot.Occupied || slot.Generation != EntityGeneration(entity))
			{
				return false;
			}
			slot.Occupied = false;
			slot.Value = TComponent();
			if (m_count > 0)
			{
				m_count -= 1;
			}
			return true;
		}

		void Destroy(_In_ Entity entity)
		{
			Remove(entity);
		}

		UINT32 getCount() const
		{
			return m_count;
		}

	private:
		void Ensure(_In_ UINT32 index)
		{
			if (index >= m_slots.size())
			{
				m_slots.resize(static_cast<size_t>(index) + 1u);
			}
		}

		struct Slot
		{
			UINT32 Generation;
			bool Occupied;
			TComponent Value;

			Slot()
				: Generation(0)
				, Occupied(false)
			{
			}
		};

		std::vector<Slot>	m_slots;
		UINT32				m_count;
	};

	class World
	{
	public:
		World();
		~World();

		World(_In_ World const&) = delete;
		World& operator=(_In_ World const&) = delete;

		Entity CreateEntity();
		Entity CreateEntity(_In_z_ const char* name);
		void DestroyEntity(_In_ Entity entity);
		bool isAlive(_In_ Entity entity) const;

		void setName(_In_ Entity entity, _In_z_ const char* name);
		const char* getName(_In_ Entity entity) const;

		UINT32 getAliveCount() const;
		void GetAliveEntities(_Out_ std::vector<Entity>* entities) const;

		Transform* AddTransform(_In_ Entity entity, _In_ Transform const& value);
		Transform* getTransform(_In_ Entity entity);
		Transform const* getTransform(_In_ Entity entity) const;
		bool RemoveTransform(_In_ Entity entity);

		CameraComponent* AddCamera(_In_ Entity entity, _In_ CameraComponent const& value);
		CameraComponent* getCamera(_In_ Entity entity);
		CameraComponent const* getCamera(_In_ Entity entity) const;
		bool RemoveCamera(_In_ Entity entity);

		MeshRenderer* AddMeshRenderer(_In_ Entity entity, _In_ MeshRenderer const& value);
		MeshRenderer* getMeshRenderer(_In_ Entity entity);
		MeshRenderer const* getMeshRenderer(_In_ Entity entity) const;
		bool RemoveMeshRenderer(_In_ Entity entity);

		RigidBody* AddRigidBody(_In_ Entity entity, _In_ RigidBody const& value);
		RigidBody* getRigidBody(_In_ Entity entity);
		RigidBody const* getRigidBody(_In_ Entity entity) const;
		bool RemoveRigidBody(_In_ Entity entity);

		Collider* AddCollider(_In_ Entity entity, _In_ Collider const& value);
		Collider* getCollider(_In_ Entity entity);
		Collider const* getCollider(_In_ Entity entity) const;
		bool RemoveCollider(_In_ Entity entity);

		AudioSourceComponent* AddAudioSource(_In_ Entity entity, _In_ AudioSourceComponent const& value);
		AudioSourceComponent* getAudioSource(_In_ Entity entity);
		AudioSourceComponent const* getAudioSource(_In_ Entity entity) const;
		bool RemoveAudioSource(_In_ Entity entity);

		ScriptComponent* AddScript(_In_ Entity entity, _In_ ScriptComponent const& value);
		ScriptComponent* getScript(_In_ Entity entity);
		ScriptComponent const* getScript(_In_ Entity entity) const;
		bool RemoveScript(_In_ Entity entity);

		Light* AddLight(_In_ Entity entity, _In_ Light const& value);
		Light* getLight(_In_ Entity entity);
		Light const* getLight(_In_ Entity entity) const;
		bool RemoveLight(_In_ Entity entity);

		PrefabComponent* AddPrefab(_In_ Entity entity, _In_ PrefabComponent const& value);
		PrefabComponent* getPrefab(_In_ Entity entity);
		PrefabComponent const* getPrefab(_In_ Entity entity) const;
		bool RemovePrefab(_In_ Entity entity);

		Entity SpawnPrefab(_In_ Prefab const& prefab);
		void Clear();

	private:
		struct Record
		{
			UINT32 Generation;
			bool Alive;
			std::string Name;

			Record()
				: Generation(1)
				, Alive(false)
			{
			}
		};

		bool Valid(_In_ Entity entity) const;

		std::vector<Record>							m_records;
		std::vector<UINT32>							m_free;
		UINT32										m_aliveCount;

		ComponentStorage<Transform>					m_transforms;
		ComponentStorage<CameraComponent>			m_cameras;
		ComponentStorage<MeshRenderer>				m_meshRenderers;
		ComponentStorage<RigidBody>					m_rigidBodies;
		ComponentStorage<Collider>					m_colliders;
		ComponentStorage<AudioSourceComponent>		m_audioSources;
		ComponentStorage<ScriptComponent>			m_scripts;
		ComponentStorage<Light>						m_lights;
		ComponentStorage<PrefabComponent>			m_prefabs;
	};
}

#endif
