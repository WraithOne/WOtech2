////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputGamepad.cpp
///
///			Description:
///
///			Created:	01.05.2014
///			Edited:		28.08.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"

namespace WOtech
{
	void InputManager::ScanGamePad()
	{
		UINT index = MAX_PLAYER_COUNT - 1;// gamepad index is 0-7
		for (UINT i = 0; i <= index; i++)
		{
			if (i < winrt::Windows::Gaming::Input::Gamepad::Gamepads().Size())
			{
				m_gamePad->at(i) = winrt::Windows::Gaming::Input::Gamepad::Gamepads().GetAt(i);
			}
			else
			{
				m_gamePad->at(i) = nullptr;
			}
		}
	}

	bool InputManager::GamepadConnected(_In_ GAMEPAD_INDEX const& PlayerIndex)
	{
		if (m_gamePad->at((UINT)PlayerIndex))
		{
			return true;
		}

		return false;
	}

	Gamepad_State InputManager::GamepadState(_In_ GAMEPAD_INDEX const& PlayerIndex)
	{
		if (m_gamePad->at((UINT)PlayerIndex))
		{
			auto reading = m_gamePad->at((UINT)PlayerIndex).GetCurrentReading();
			auto BatterieState = m_gamePad->at((UINT)PlayerIndex).TryGetBatteryReport();
			//auto maxcharge = BatterieState->FullChargeCapacityInMilliwattHours();
			auto currentcharge = BatterieState.RemainingCapacityInMilliwattHours();
			Gamepad_State state;

			state.Connected = true;
			state.isWireless = m_gamePad->at((UINT)PlayerIndex).IsWireless();
			state.ChargePercentage = currentcharge.Value() / 10; // todo: compute the correct percentage

			state.TimeStamp = reading.Timestamp;
			state.Buttons.A = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::A) == winrt::Windows::Gaming::Input::GamepadButtons::A;
			state.Buttons.B = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::B) == winrt::Windows::Gaming::Input::GamepadButtons::B;
			state.Buttons.X = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::X) == winrt::Windows::Gaming::Input::GamepadButtons::X;
			state.Buttons.Y = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::Y) == winrt::Windows::Gaming::Input::GamepadButtons::Y;
			state.Buttons.LeftStick = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::LeftThumbstick) == winrt::Windows::Gaming::Input::GamepadButtons::LeftThumbstick;
			state.Buttons.RightStick = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::RightThumbstick) == winrt::Windows::Gaming::Input::GamepadButtons::RightThumbstick;
			state.Buttons.LeftShoulder = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::LeftShoulder) == winrt::Windows::Gaming::Input::GamepadButtons::LeftShoulder;
			state.Buttons.RightShoulder = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::RightShoulder) == winrt::Windows::Gaming::Input::GamepadButtons::RightShoulder;
			state.Buttons.View = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::View) == winrt::Windows::Gaming::Input::GamepadButtons::View;
			state.Buttons.Menu = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::Menu) == winrt::Windows::Gaming::Input::GamepadButtons::Menu;

			state.DPad.Up = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::DPadUp) == winrt::Windows::Gaming::Input::GamepadButtons::DPadUp;
			state.DPad.Down = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::DPadDown) == winrt::Windows::Gaming::Input::GamepadButtons::DPadDown;
			state.DPad.Left = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::DPadLeft) == winrt::Windows::Gaming::Input::GamepadButtons::DPadLeft;
			state.DPad.Right = (reading.Buttons & winrt::Windows::Gaming::Input::GamepadButtons::DPadRight) == winrt::Windows::Gaming::Input::GamepadButtons::DPadRight;

			state.Tumbsticks.LeftX = reading.LeftThumbstickX;
			state.Tumbsticks.LeftY = reading.LeftThumbstickY;
			state.Tumbsticks.RightX = reading.RightThumbstickX;
			state.Tumbsticks.RightY = reading.RightThumbstickY;

			state.Triggers.Left = reading.LeftTrigger;
			state.Triggers.Right = reading.RightTrigger;

			return state;
		}
		return Gamepad_State();
	}

	void InputManager::GamepadSetVibration(_In_ GAMEPAD_INDEX const& PlayerIndex, _In_ winrt::Windows::Gaming::Input::GamepadVibration const& Vibration)
	{
		if (m_gamePad->at((UINT)PlayerIndex))
		{
			(m_gamePad->at((UINT)PlayerIndex)).Vibration(Vibration);
		}
	}// InputClass GamePadSetVibration
} // namespace WOtech