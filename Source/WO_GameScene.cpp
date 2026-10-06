////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: GameScene.cpp
///
///			Description:
///
///			Created:	10.04.2016
///			Edited:		01.02.2017
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_GameScene.hpp"
#include "WO_Scene.hpp"

namespace WOtech
{
	GameScene::GameScene()
	{
	}

	World& GameScene::getWorld()
	{
		return m_world;
	}

	World const& GameScene::getWorld() const
	{
		return m_world;
	}

	void GameScene::Update(_In_ GameTime const& gametime)
	{
		m_physics.Step(&m_world, gametime.DeltaTime);
		m_scripts.Tick(&m_world, &m_physics, gametime.DeltaTime);
	}

	bool GameScene::Save(_In_z_ const char* path)
	{
		return SceneDocument::Save(m_world, path, nullptr);
	}

	bool GameScene::Load(_In_z_ const char* path)
	{
		return SceneDocument::Load(&m_world, path, nullptr);
	}
}