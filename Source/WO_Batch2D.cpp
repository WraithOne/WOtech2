////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Batch2D.cpp
///
///			Description:
///
///			Created:	01.11.2017
///			Edited:		28.08.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Batch2D.hpp"

namespace WOtech
{
	Batch2D_Text::Batch2D_Text(_In_ WOtech::TextBlock* const& text, _In_ UINT const& ID, _In_ FLOAT const& depth)
	{
		m_text = text;

		m_batchID = ID;
		m_depth = depth;
	}

	UINT Batch2D_Text::getID()
	{
		return m_batchID;
	}
	FLOAT Batch2D_Text::getDepth()
	{
		return m_depth;
	}
	WOtech::TextBlock* Batch2D_Text::getText()
	{
		return m_text;
	}
}