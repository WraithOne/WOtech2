////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: GameSceneObject.h
///
///			Description:
///
///			Created:	10.04.2016
///			Edited:		05.01.2019
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_GAMESCENEOBJECT_H
#define WO_GAMESCENEOBJECT_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	interface IGameSceneObject
	{
	public:
		virtual UINT getID() = 0;
	};
}
#endif