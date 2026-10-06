////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: GameScene.h
///
///			Description:
///
///			Created:	10.04.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_GAMESCENE_H
#define WO_GAMESCENE_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "WO_World.hpp"
#include "WO_IGame.hpp"
#include "WO_Physics.hpp"
#include "WO_Script.hpp"

namespace WOtech
{
	class GameScene
	{
	public:
		GameScene();

		World& getWorld();
		World const& getWorld() const;

		void Update(_In_ GameTime const& gametime);
		bool Save(_In_z_ const char* path);
		bool Load(_In_z_ const char* path);

	private:
		World			m_world;
		PhysicsWorld	m_physics;
		ScriptHost		m_scripts;
	};
}
#endif
