////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputPen.cpp
///
///			Description:
///
///			Created:	01.05.2014
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"

namespace WOtech
{
	bool InputManager::PenConnected()
	{
		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Pen)
				return true;
		}
		return false;
	}

	winrt::com_array<Pen_State> InputManager::getPenState()
	{
		auto temp = winrt::com_array<Pen_State>(m_touchCapabilities.Contacts()); // TODO: max pen supportet
		UINT nr = 0;
		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Pen)
			{
				temp[nr].PointerID = it->PointerId();
				temp[nr].BarrelButton = it->Properties().IsBarrelButtonPressed();
				temp[nr].isErazer = it->Properties().IsEraser();
				temp[nr].Position.X = it->Position().X;
				temp[nr].Position.Y = it->Position().Y;
				temp[nr].Pressure = it->Properties().Pressure();
			}
		}
		return temp;
	}
}//namespace WOtech