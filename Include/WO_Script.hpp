////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Script.hpp
///
///			Description:
///			Sandboxed Lua 5.4 host. os, io, and package are not opened.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_SCRIPT_H
#define WO_SCRIPT_H

//////////////
// INCLUDES //
//////////////
#include "WO_World.hpp"

namespace WOtech
{
	class PhysicsWorld;

	struct ScriptState;

	class ScriptHost
	{
	public:
		ScriptHost();
		~ScriptHost();

		ScriptHost(_In_ ScriptHost const&) = delete;
		ScriptHost& operator=(_In_ ScriptHost const&) = delete;

		void Tick(_In_ World* world, _In_opt_ PhysicsWorld* physics, _In_ float dt);
		bool RunSource(_In_ World* world, _In_z_ const char* source, _Out_opt_ std::string* error);

	private:
		ScriptState*	m_impl;
	};
}

#endif
