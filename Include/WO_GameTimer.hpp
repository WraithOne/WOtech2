////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Gametimer.h
///
///			Description:
///
///			Created:	12.05.2014
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_GAMETIMER_H
#define WO_GAMETIMER_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	struct GameTime
	{
		FLOAT DeltaTime; // Return the Delta time between the last two updates
		FLOAT PlayingTime; // Return the Elapsed time the Game has been active in seconds since Reset
	};

	class GameTimer
	{
	public:
		GameTimer();

		GameTime GetTime();

		void PlayingTime(_In_ FLOAT const& time);	// Set the Elapsed playing time -- used for restarting in the middle of a game

		void Reset();
		void Start();
		void Stop();
		void Update();

		bool Active();

	private:
		FLOAT			m_secondsPerCount;	// 1.0 / Frequency
		FLOAT			m_deltaTime;

		LARGE_INTEGER	m_baseTime;
		LARGE_INTEGER	m_pausedTime;
		LARGE_INTEGER	m_stopTime;
		LARGE_INTEGER	m_previousTime;
		LARGE_INTEGER	m_currentTime;

		bool m_active;
	};
}
#endif