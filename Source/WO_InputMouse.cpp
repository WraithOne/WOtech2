////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputMouse.cpp
///
///			Description:
///
///			Created:	28.08.2015
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"
#include "WO_Window.hpp"

namespace WOtech
{
	bool InputManager::MouseConnected()
	{
		if (m_mouseCapabilities.MousePresent() >= 1)
			return true;

		return false;
	}
	Mouse_State InputManager::getMouseState()
	{
		Mouse_State temp{};
		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Mouse)
			{
				temp.PointerID = it->PointerId();
				temp.Position = m_mouseDelta;
				temp.Buttons.LeftButton = it->Properties().IsLeftButtonPressed();
				temp.Buttons.RightButton = it->Properties().IsRightButtonPressed();
				temp.Buttons.MiddleButton = it->Properties().IsMiddleButtonPressed();
				temp.Buttons.X1Button = it->Properties().IsXButton1Pressed();
				temp.Buttons.X2Button = it->Properties().IsXButton2Pressed();
				temp.WheelDelta = it->Properties().MouseWheelDelta();
			}
		}
		return temp;
	}

	void InputManager::MouseShowCursor(_In_ bool const& Show)
	{
		auto coreWindow = m_window->getCoreWindow();
		if (Show)
		{
			coreWindow.PointerCursor(winrt::Windows::UI::Core::CoreCursor(winrt::Windows::UI::Core::CoreCursorType::Arrow, 0));
		}
		else

		{
			coreWindow.PointerCursor(nullptr);
		}
	}
	bool InputManager::MouseCursorVisible()
	{
		auto coreWindow = m_window->getCoreWindow();

		if (coreWindow.PointerCursor() == nullptr)
			return false;

		return true;
	}
}// namespace WOtech