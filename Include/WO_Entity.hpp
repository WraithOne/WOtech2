////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Entity.hpp
///
///			Description:
///			Stable entity identifier. Low 32 bits are the slot index.
///			High 32 bits are the generation. Zero is never a live entity.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_ENTITY_H
#define WO_ENTITY_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	typedef UINT64 Entity;

	static const Entity kInvalidEntity = 0;

	inline UINT32 EntityIndex(_In_ Entity entity)
	{
		return static_cast<UINT32>(entity & 0xFFFFFFFFu);
	}

	inline UINT32 EntityGeneration(_In_ Entity entity)
	{
		return static_cast<UINT32>(entity >> 32);
	}

	inline Entity MakeEntity(_In_ UINT32 index, _In_ UINT32 generation)
	{
		return (static_cast<Entity>(generation) << 32) | static_cast<Entity>(index);
	}
}

#endif
