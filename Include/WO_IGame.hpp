////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: IGAME.h
///
///			Description:
///
///			Created:	14.01.2018
///			Edited:		28.08.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_IGAME_H
#define WO_IGAME_H

//////////////
// INCLUDES //
//////////////
#include "..\Include\WO_GameTimer.hpp"

namespace WOtech
{
	class IGame
	{
	public:
		virtual void Initalize() = 0;
		virtual void Load() = 0;
		virtual void UnLoad() = 0;
		virtual void Update(_In_ GameTime const& gametime) = 0;
		virtual void Draw(_In_ GameTime const& gametime) = 0;

		virtual void OnWindowChanged() = 0;
		virtual void OnSuspending() = 0;
		virtual void OnResuming() = 0;
	};
}
#endif