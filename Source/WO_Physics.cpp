////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Physics.cpp
///
///			Description:
///			box3d-backed rigid body step. Components stay engine-owned.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Physics.hpp"

#include "box3d/box3d.h"

#include <vector>

namespace WOtech
{
	struct BodySlot
	{
		b3BodyId Body;
		UINT32 Generation;
		bool Live;

		BodySlot()
			: Body(b3_nullBodyId)
			, Generation(0)
			, Live(false)
		{
		}
	};

	struct PhysicsWorld::Impl
	{
		b3WorldId World;
		bool Grounded;
		UINT32 Contacts;
		std::vector<BodySlot> Slots;

		Impl()
			: World(b3_nullWorldId)
			, Grounded(false)
			, Contacts(0)
		{
			b3WorldDef def = b3DefaultWorldDef();
			def.gravity = { 0.0f, -9.81f, 0.0f };
			World = b3CreateWorld(&def);
		}
	};

	static b3BodyType ToBoxType(_In_ BODY_TYPE type)
	{
		switch (type)
		{
		case BODY_TYPE_STATIC: return b3_staticBody;
		case BODY_TYPE_KINEMATIC: return b3_kinematicBody;
		default: return b3_dynamicBody;
		}
	}

	PhysicsWorld::PhysicsWorld()
		: m_impl(new Impl())
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
		if (m_impl != nullptr && b3World_IsValid(m_impl->World))
		{
			b3DestroyWorld(m_impl->World);
		}
		delete m_impl;
		m_impl = nullptr;
	}

	void PhysicsWorld::Step(_In_ World* world, _In_ float dt)
	{
		if (m_impl == nullptr || world == nullptr || !b3World_IsValid(m_impl->World))
		{
			return;
		}
		if (dt < 0.0f)
		{
			dt = 0.0f;
		}
		if (dt > 0.05f)
		{
			dt = 0.05f;
		}

		if (!m_impl->Grounded)
		{
			b3BodyDef bodyDef = b3DefaultBodyDef();
			bodyDef.type = b3_staticBody;
			bodyDef.position = { 0.0f, -0.5f, 0.0f };
			b3BodyId ground = b3CreateBody(m_impl->World, &bodyDef);
			b3ShapeDef shapeDef = b3DefaultShapeDef();
			b3BoxHull hull = b3MakeBoxHull(50.0f, 0.5f, 50.0f);
			b3CreateHullShape(ground, &shapeDef, &hull.base);
			m_impl->Grounded = true;
		}

		std::vector<Entity> alive;
		world->GetAliveEntities(&alive);
		for (size_t i = 0; i < alive.size(); ++i)
		{
			Entity const entity = alive[i];
			RigidBody* body = world->getRigidBody(entity);
			Collider* collider = world->getCollider(entity);
			Transform* transform = world->getTransform(entity);
			if (body == nullptr || collider == nullptr || transform == nullptr)
			{
				continue;
			}
			UINT32 const index = EntityIndex(entity);
			if (index >= m_impl->Slots.size())
			{
				m_impl->Slots.resize(static_cast<size_t>(index) + 1u);
			}
			BodySlot& slot = m_impl->Slots[index];
			if (!slot.Live || slot.Generation != EntityGeneration(entity) || !b3Body_IsValid(slot.Body))
			{
				b3BodyDef bodyDef = b3DefaultBodyDef();
				bodyDef.type = ToBoxType(body->Type);
				bodyDef.position = { transform->Position.x, transform->Position.y, transform->Position.z };
				bodyDef.gravityScale = body->UseGravity ? 1.0f : 0.0f;
				slot.Body = b3CreateBody(m_impl->World, &bodyDef);
				b3ShapeDef shapeDef = b3DefaultShapeDef();
				shapeDef.isSensor = collider->IsSensor;
				if (collider->Shape == COLLIDER_SHAPE_SPHERE)
				{
					b3Sphere sphere = { { 0.0f, 0.0f, 0.0f }, collider->Radius };
					b3CreateSphereShape(slot.Body, &shapeDef, &sphere);
				}
				else if (collider->Shape == COLLIDER_SHAPE_CAPSULE)
				{
					float const half = collider->Height * 0.5f;
					b3Capsule capsule = { { 0.0f, -half, 0.0f }, { 0.0f, half, 0.0f }, collider->Radius };
					b3CreateCapsuleShape(slot.Body, &shapeDef, &capsule);
				}
				else
				{
					b3BoxHull hull = b3MakeBoxHull(collider->HalfExtents.x, collider->HalfExtents.y, collider->HalfExtents.z);
					b3CreateHullShape(slot.Body, &shapeDef, &hull.base);
				}
				b3Body_ApplyMassFromShapes(slot.Body);
				slot.Generation = EntityGeneration(entity);
				slot.Live = true;
				b3Vec3 velocity = { body->LinearVelocity.x, body->LinearVelocity.y, body->LinearVelocity.z };
				b3Body_SetLinearVelocity(slot.Body, velocity);
			}
		}

		if (dt > 0.0f)
		{
			b3World_Step(m_impl->World, dt, 4);
		}

		b3ContactEvents events = b3World_GetContactEvents(m_impl->World);
		m_impl->Contacts = static_cast<UINT32>(events.beginCount);

		for (size_t i = 0; i < alive.size(); ++i)
		{
			Entity const entity = alive[i];
			UINT32 const index = EntityIndex(entity);
			if (index >= m_impl->Slots.size() || !m_impl->Slots[index].Live)
			{
				continue;
			}
			BodySlot const& slot = m_impl->Slots[index];
			if (!b3Body_IsValid(slot.Body) || b3Body_GetType(slot.Body) == b3_staticBody)
			{
				continue;
			}
			b3Pos const position = b3Body_GetPosition(slot.Body);
			b3Vec3 const velocity = b3Body_GetLinearVelocity(slot.Body);
			Transform* transform = world->getTransform(entity);
			RigidBody* body = world->getRigidBody(entity);
			if (transform != nullptr)
			{
				transform->Position = DirectX::XMFLOAT3(position.x, position.y, position.z);
			}
			if (body != nullptr)
			{
				body->LinearVelocity = DirectX::XMFLOAT3(velocity.x, velocity.y, velocity.z);
				body->PhysicsHandle = static_cast<UINT64>(index) + 1u;
			}
		}
	}

	void PhysicsWorld::SetLinearVelocity(_In_ World* world, _In_ Entity entity, _In_ DirectX::XMFLOAT3 const& velocity)
	{
		if (world == nullptr || m_impl == nullptr)
		{
			return;
		}
		RigidBody* body = world->getRigidBody(entity);
		if (body != nullptr)
		{
			body->LinearVelocity = velocity;
		}
		UINT32 const index = EntityIndex(entity);
		if (index < m_impl->Slots.size() && m_impl->Slots[index].Live && b3Body_IsValid(m_impl->Slots[index].Body))
		{
			b3Vec3 value = { velocity.x, velocity.y, velocity.z };
			b3Body_SetLinearVelocity(m_impl->Slots[index].Body, value);
		}
	}

	void PhysicsWorld::ApplyForce(_In_ World* world, _In_ Entity entity, _In_ DirectX::XMFLOAT3 const& force)
	{
		UNREFERENCED_PARAMETER(world);
		if (m_impl == nullptr)
		{
			return;
		}
		UINT32 const index = EntityIndex(entity);
		if (index < m_impl->Slots.size() && m_impl->Slots[index].Live && b3Body_IsValid(m_impl->Slots[index].Body))
		{
			b3Vec3 value = { force.x, force.y, force.z };
			b3Body_ApplyForceToCenter(m_impl->Slots[index].Body, value, true);
		}
	}

	UINT32 PhysicsWorld::getContactCount() const
	{
		return m_impl != nullptr ? m_impl->Contacts : 0;
	}
}
