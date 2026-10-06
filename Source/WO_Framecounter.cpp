////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Framecounter.cpp
///
///			Description:
///
///			Created:	06.09.2014
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Framecounter.hpp"

namespace WOtech
{
	Framecounter::Framecounter()
	{
		m_fps = 0;
		m_fpscount = 0;
	}

	void Framecounter::update()
	{
		m_fpscount++;

		if (m_fpsinterval.value() > 1000)
		{
			m_fps = m_fpscount;

			m_fpscount = 0;

			m_fpsinterval = Interval();
		}
		std::wstring wstring = L"FPS: " + std::to_wstring(m_fps);

		m_string = winrt::hstring(wstring.c_str());
	}

	UINT Framecounter::get()
	{
		return m_fps;
	}

	winrt::hstring Framecounter::getString()
	{
		return m_string;
	}
}