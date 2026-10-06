////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Framecounter.h
///
///			Description:
///
///			Created:	16.04.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_FRAMECOUNTER_H
#define WO_FRAMECOUNTER_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	class Interval
	{
	private:
		ULONGLONG initial_;

	public:
		inline Interval() : initial_(GetTickCount64())
		{
		}

		virtual ~Interval()
		{
		}

		inline ULONGLONG value() const
		{
			return GetTickCount64() - initial_;
		}
	};

	class Framecounter
	{
	public:
		Framecounter();
		void			update();

		UINT			get();
		winrt::hstring	getString();

	private:
		UINT			m_fps;
		UINT			m_fpscount;
		Interval		m_fpsinterval;
		winrt::hstring	m_string;
	};
}

#endif