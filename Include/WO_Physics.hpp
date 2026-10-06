////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Physics.hpp
///
///			Description:
///			Physics wrapper. Callers never see box3d types.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_PHYSICS_H
#define WO_PHYSICS_H

//////////////
// INCLUDES //
//////////////
#include "WO_World.hpp"

namespace WOtech
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		PhysicsWorld(_In_ PhysicsWorld const&) = delete;
		PhysicsWorld& operator=(_In_ PhysicsWorld const&) = delete;

		void Step(_In_ World* world, _In_ float dt);
		void SetLinearVelocity(_In_ World* world, _In_ Entity entity, _In_ DirectX::XMFLOAT3 const& velocity);
		void ApplyForce(_In_ World* world, _In_ Entity entity, _In_ DirectX::XMFLOAT3 const& force);
		UINT32 getContactCount() const;

	private:
		struct Impl;
		Impl*	m_impl;
	};
}

#endif
