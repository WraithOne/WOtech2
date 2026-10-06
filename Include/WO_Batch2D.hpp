////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Batch2D.h
///
///			Description:
///
///			Created:	28.08.2018
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_BATCH2D_H
#define WO_BATCH2D_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "..\Include\WO_2DComponents.hpp"

namespace WOtech
{
	interface IBatch2D
	{
		virtual UINT getID() = 0;
		virtual FLOAT getDepth() = 0;
	};

	class Batch2D_Text : public IBatch2D
	{
	public:
		Batch2D_Text(_In_ WOtech::TextBlock* const& text, _In_ UINT const& ID, _In_ FLOAT const& depth);

		virtual UINT getID();
		virtual FLOAT getDepth();
		WOtech::TextBlock* getText();

	private:
		UINT				m_batchID;
		FLOAT				m_depth;
		WOtech::TextBlock*	m_text;
	};
}
#endif