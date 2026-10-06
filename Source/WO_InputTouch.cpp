////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputTouch.cpp
///
///			Description:
///
///			Created:	27.08.2016
///			Edited:		01.06.2017
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"

namespace WOtech
{
	bool InputManager::TouchConnected()
	{
		if (m_touchCapabilities.TouchPresent() != 0)
			return true;
		else
			return false;
	}
	winrt::com_array<Touch_State> InputManager::getTouchState()
	{
		auto output = winrt::com_array<Touch_State>(m_touchCapabilities.Contacts());//TODO : max contacts supportet
		auto nr = 0U;

		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Touch)
			{
				Touch_State temp{ it->PointerId() ,Pointer_Position{it->Position().X , it->Position().Y} };
				output[nr] = temp;
			}
			nr++;
		}
		return output;
	}
}