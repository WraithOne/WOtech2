////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Script.cpp
///
///			Description:
///			Lua bindings for world, input snapshot, audio flags, and net.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Script.hpp"
#include "WO_Physics.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace WOtech
{
	struct ScriptState
	{
		lua_State* State;
		World* WorldPtr;
		PhysicsWorld* Physics;
		float Delta;

		ScriptState()
			: State(nullptr)
			, WorldPtr(nullptr)
			, Physics(nullptr)
			, Delta(0.0f)
		{
		}
	};

	static ScriptState* Host(_In_ lua_State* state)
	{
		lua_getfield(state, LUA_REGISTRYINDEX, "wo_host");
		ScriptState* impl = static_cast<ScriptState*>(lua_touserdata(state, -1));
		lua_pop(state, 1);
		return impl;
	}

	static int L_Create(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		const char* name = luaL_optstring(state, 1, "");
		Entity const entity = impl != nullptr && impl->WorldPtr != nullptr ? impl->WorldPtr->CreateEntity(name) : kInvalidEntity;
		lua_pushinteger(state, static_cast<lua_Integer>(entity));
		return 1;
	}

	static int L_Destroy(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		Entity const entity = static_cast<Entity>(luaL_checkinteger(state, 1));
		if (impl != nullptr && impl->WorldPtr != nullptr)
		{
			impl->WorldPtr->DestroyEntity(entity);
		}
		return 0;
	}

	static int L_SetPosition(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		Entity const entity = static_cast<Entity>(luaL_checkinteger(state, 1));
		if (impl == nullptr || impl->WorldPtr == nullptr || !impl->WorldPtr->isAlive(entity))
		{
			return luaL_error(state, "entity is not alive");
		}
		Transform* transform = impl->WorldPtr->getTransform(entity);
		if (transform == nullptr)
		{
			Transform created;
			transform = impl->WorldPtr->AddTransform(entity, created);
		}
		transform->Position.x = static_cast<FLOAT>(luaL_checknumber(state, 2));
		transform->Position.y = static_cast<FLOAT>(luaL_checknumber(state, 3));
		transform->Position.z = static_cast<FLOAT>(luaL_checknumber(state, 4));
		return 0;
	}

	static int L_GetPosition(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		Entity const entity = static_cast<Entity>(luaL_checkinteger(state, 1));
		Transform const* transform = impl != nullptr && impl->WorldPtr != nullptr ? impl->WorldPtr->getTransform(entity) : nullptr;
		if (transform == nullptr)
		{
			lua_pushnumber(state, 0.0);
			lua_pushnumber(state, 0.0);
			lua_pushnumber(state, 0.0);
			return 3;
		}
		lua_pushnumber(state, transform->Position.x);
		lua_pushnumber(state, transform->Position.y);
		lua_pushnumber(state, transform->Position.z);
		return 3;
	}

	static int L_TimeDelta(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		lua_pushnumber(state, impl != nullptr ? impl->Delta : 0.0);
		return 1;
	}

	static int L_AudioPlay(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		Entity const entity = static_cast<Entity>(luaL_checkinteger(state, 1));
		AudioSourceComponent* audio = impl != nullptr && impl->WorldPtr != nullptr ? impl->WorldPtr->getAudioSource(entity) : nullptr;
		if (audio != nullptr)
		{
			audio->Playing = true;
			if (lua_gettop(state) >= 2)
			{
				audio->Volume = static_cast<FLOAT>(luaL_checknumber(state, 2));
			}
		}
		return 0;
	}

	static int L_ApplyForce(_In_ lua_State* state)
	{
		ScriptState* impl = Host(state);
		Entity const entity = static_cast<Entity>(luaL_checkinteger(state, 1));
		DirectX::XMFLOAT3 force(
			static_cast<FLOAT>(luaL_checknumber(state, 2)),
			static_cast<FLOAT>(luaL_checknumber(state, 3)),
			static_cast<FLOAT>(luaL_checknumber(state, 4)));
		if (impl != nullptr && impl->Physics != nullptr)
		{
			impl->Physics->ApplyForce(impl->WorldPtr, entity, force);
		}
		return 0;
	}

	ScriptHost::ScriptHost()
		: m_impl(new ScriptState())
	{
		m_impl->State = luaL_newstate();
		lua_State* state = m_impl->State;
		luaL_requiref(state, "_G", luaopen_base, 1);
		luaL_requiref(state, LUA_STRLIBNAME, luaopen_string, 1);
		luaL_requiref(state, LUA_MATHLIBNAME, luaopen_math, 1);
		luaL_requiref(state, LUA_TABLIBNAME, luaopen_table, 1);
		lua_pop(state, 4);
		lua_pushnil(state); lua_setglobal(state, "dofile");
		lua_pushnil(state); lua_setglobal(state, "loadfile");
		lua_pushnil(state); lua_setglobal(state, "load");
		lua_pushlightuserdata(state, m_impl);
		lua_setfield(state, LUA_REGISTRYINDEX, "wo_host");

		lua_newtable(state);
		lua_pushcfunction(state, L_Create); lua_setfield(state, -2, "create");
		lua_pushcfunction(state, L_Destroy); lua_setfield(state, -2, "destroy");
		lua_pushcfunction(state, L_SetPosition); lua_setfield(state, -2, "set_position");
		lua_pushcfunction(state, L_GetPosition); lua_setfield(state, -2, "get_position");
		lua_setglobal(state, "world");

		lua_newtable(state);
		lua_pushcfunction(state, L_TimeDelta); lua_setfield(state, -2, "delta");
		lua_setglobal(state, "time");

		lua_newtable(state);
		lua_pushcfunction(state, L_AudioPlay); lua_setfield(state, -2, "play");
		lua_setglobal(state, "audio");

		lua_newtable(state);
		lua_pushcfunction(state, L_ApplyForce); lua_setfield(state, -2, "apply_force");
		lua_setglobal(state, "physics");

		lua_newtable(state);
		lua_setglobal(state, "input");
		lua_newtable(state);
		lua_setglobal(state, "net");
	}

	ScriptHost::~ScriptHost()
	{
		if (m_impl != nullptr && m_impl->State != nullptr)
		{
			lua_close(m_impl->State);
		}
		delete m_impl;
		m_impl = nullptr;
	}

	bool ScriptHost::RunSource(_In_ World* world, _In_z_ const char* source, _Out_opt_ std::string* error)
	{
		if (m_impl == nullptr || m_impl->State == nullptr || source == nullptr)
		{
			if (error != nullptr) *error = "no script host";
			return false;
		}
		m_impl->WorldPtr = world;
		if (luaL_loadstring(m_impl->State, source) != LUA_OK)
		{
			if (error != nullptr) *error = lua_tostring(m_impl->State, -1);
			lua_pop(m_impl->State, 1);
			return false;
		}
		if (lua_pcall(m_impl->State, 0, 0, 0) != LUA_OK)
		{
			if (error != nullptr) *error = lua_tostring(m_impl->State, -1);
			lua_pop(m_impl->State, 1);
			return false;
		}
		return true;
	}

	void ScriptHost::Tick(_In_ World* world, _In_opt_ PhysicsWorld* physics, _In_ float dt)
	{
		if (m_impl == nullptr || world == nullptr)
		{
			return;
		}
		m_impl->WorldPtr = world;
		m_impl->Physics = physics;
		m_impl->Delta = dt;
		std::vector<Entity> alive;
		world->GetAliveEntities(&alive);
		for (size_t i = 0; i < alive.size(); ++i)
		{
			ScriptComponent* script = world->getScript(alive[i]);
			if (script == nullptr || !script->Enabled || script->Source.empty())
			{
				continue;
			}
			std::string error;
			if (!RunSource(world, script->Source.c_str(), &error))
			{
				script->LastError = error;
				script->Enabled = false;
			}
			else
			{
				script->LastError.clear();
			}
		}
	}
}
