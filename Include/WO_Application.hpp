////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: Application.h
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		07.08.2021
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_APPLICATION_H
#define WO_APPLICATION_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_IGame.hpp"

namespace WOtech
{
#ifndef WO_EDITOR
#define WO_EDITOR 0
#endif

	class Application
	{
	public:
		Application();
		~Application();

		int Run(_In_ IGame* game);
		void RunHeadless(_In_ IGame* game, _In_ UINT32 frames);
		void RequestExit();

		GameTimer& getTimer();
		bool isEditorBuild() const;

	private:
		void Tick(_In_ IGame* game);

		GameTimer	m_timer;
		bool		m_running;
	};
} // namespace WOtech

#endif