////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: Application.cpp
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		07.08.2021
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Application.hpp"

namespace WOtech
{
	Application::Application()
		: m_running(false)
	{
	}

	Application::~Application()
	{
		m_running = false;
	}

	int Application::Run(_In_ IGame* game)
	{
		if (game == nullptr)
		{
			return 1;
		}
		game->Initalize();
		game->Load();
		m_running = true;
		m_timer.Reset();
		m_timer.Start();
		while (m_running)
		{
			MSG message;
			while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					m_running = false;
				}
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			Tick(game);
		}
		game->UnLoad();
		return 0;
	}

	void Application::RunHeadless(_In_ IGame* game, _In_ UINT32 frames)
	{
		if (game == nullptr)
		{
			return;
		}
		game->Initalize();
		game->Load();
		m_timer.Reset();
		m_timer.Start();
		for (UINT32 i = 0; i < frames; ++i)
		{
			Tick(game);
		}
		game->UnLoad();
	}

	void Application::RequestExit()
	{
		m_running = false;
		PostQuitMessage(0);
	}

	GameTimer& Application::getTimer()
	{
		return m_timer;
	}

	bool Application::isEditorBuild() const
	{
		return WO_EDITOR != 0;
	}

	void Application::Tick(_In_ IGame* game)
	{
		m_timer.Update();
		GameTime const time = m_timer.GetTime();
		game->Update(time);
		game->Draw(time);
	}
}