////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: VirtualController.h
///
///			Description:
///
///			Created:	04.01.2017
///			Edited:		28.08.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_VirtualController.hpp"

namespace WOtech
{
	VirtualController::VirtualController(_In_ InputManager* const& input)
	{
		m_inputManager = input;

		m_state = WOtech::Virtual_Controller_State();
		m_currentInput = WOtech::CURRENT_INPUT_DEVICE::CURRENT_INPUT_DEVICE_UNSPECIFIED;
		m_currentGamepad = WOtech::GAMEPAD_INDEX::GAMEPAD_INDEX_PLAYERONE;
		m_mouseWheelbinding = WOtech::VIRTUAL_CONTROLLER_TRIGGERS::VIRTUAL_CONTROLLER_TRIGGERS_LEFT;
		m_mousebinding = WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS::VIRTUAL_CONTROLLER_THUMBSTICKS_LEFT;
		m_currentStickLeftID = 0U;
		m_currentStickRightID = 0U;
		m_currentTriggerLeftID = 0U;
		m_currentTriggerRightID = 0U;
	}

	void VirtualController::setCurrentInput(_In_ CURRENT_INPUT_DEVICE const& current)
	{
		m_currentInput = current;
	}
	CURRENT_INPUT_DEVICE VirtualController::getCurrent()
	{
		return m_currentInput;
	}
	Virtual_Controller_State VirtualController::getState()
	{
		// Update state before returning
		Update();

		return m_state;
	}

	void VirtualController::bindGamepad(_In_ GAMEPAD_INDEX const& number)
	{
		m_currentGamepad = number;
	}

	bool VirtualController::getKeyboardButtonBinding(_In_ VIRTUAL_CONTROLLER_BUTTONS const& target, _Out_ winrt::Windows::System::VirtualKey* key) const
	{
		if (key == nullptr)
		{
			return false;
		}
		std::map<VIRTUAL_CONTROLLER_BUTTONS, winrt::Windows::System::VirtualKey>::const_iterator it = m_keyboardButtonbinding.find(target);
		if (it == m_keyboardButtonbinding.end())
		{
			return false;
		}
		*key = it->second;
		return true;
	}

	void VirtualController::bindKeyboardtoButton(_In_ VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ winrt::Windows::System::VirtualKey const& key)
	{
		std::map<VIRTUAL_CONTROLLER_BUTTONS, winrt::Windows::System::VirtualKey>::iterator it;
		it = m_keyboardButtonbinding.find(target);

		if (it != m_keyboardButtonbinding.end())
		{
			m_keyboardButtonbinding[target] = key;
		}
		else
		{
			m_keyboardButtonbinding.emplace(target, key);
		}
	}
	void VirtualController::bindKeyboardtoTrigger(_In_ VIRTUAL_CONTROLLER_TRIGGERS const& target, _In_ winrt::Windows::System::VirtualKey const& key)
	{
		std::map<VIRTUAL_CONTROLLER_TRIGGERS, winrt::Windows::System::VirtualKey>::iterator it;
		it = m_keyboardTriggerbinding.find(target);

		if (it != m_keyboardTriggerbinding.end())
		{
			m_keyboardTriggerbinding[target] = key;
		}
		else
		{
			m_keyboardTriggerbinding.emplace(target, key);
		}
	}
	void VirtualController::bindKeyboardtoThumbstick(_In_ VIRTUAL_CONTROLLER_THUMBSTICKS const& target, _In_ winrt::Windows::System::VirtualKey const& keyUP, _In_ winrt::Windows::System::VirtualKey const& keyDOWN, _In_ winrt::Windows::System::VirtualKey const& keyLEFT, _In_ winrt::Windows::System::VirtualKey const& keyRIGHT)
	{
		Virtual_Stick_Keyboard temp;
		temp.Up = keyUP;
		temp.Down = keyDOWN;
		temp.Left = keyLEFT;
		temp.Right = keyRIGHT;

		std::map<VIRTUAL_CONTROLLER_THUMBSTICKS, Virtual_Stick_Keyboard>::iterator it;
		it = m_keyboardStickbinding.find(target);

		if (it != m_keyboardStickbinding.end())
		{
			m_keyboardStickbinding[target] = temp;
		}
		else
		{
			m_keyboardStickbinding.emplace(target, temp);
		}
	}

	void VirtualController::bindMousetoButton(_In_ VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ D2D1_RECT_F const& area)
	{
		std::map<VIRTUAL_CONTROLLER_BUTTONS, D2D1_RECT_F>::iterator it;
		it = m_mouseButtonbinding.find(target);

		if (it != m_mouseButtonbinding.end())
		{
			m_mouseButtonbinding[target] = area;
		}
		else
		{
			m_mouseButtonbinding.emplace(target, area);
		}
	}
	void VirtualController::bindMouseKeytoButton(_In_ VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ WOtech::VIRTUALKEY_MOUSE const& key)
	{
		std::map<VIRTUAL_CONTROLLER_BUTTONS, WOtech::VIRTUALKEY_MOUSE>::iterator it;
		it = m_mouseKeybinding.find(target);

		if (it != m_mouseKeybinding.end())
		{
			m_mouseKeybinding[target] = key;
		}
		else
		{
			m_mouseKeybinding.emplace(target, key);
		}
	}
	void VirtualController::bindMouseWheeltoTrigger(_In_ VIRTUAL_CONTROLLER_TRIGGERS const& target)
	{
		m_mouseWheelbinding = target;
	}
	void VirtualController::bindMousetoThumbstick(_In_ VIRTUAL_CONTROLLER_THUMBSTICKS const& target)
	{
		m_mousebinding = target;
	}

	void VirtualController::bindTouchtoButton(_In_ VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ D2D1_RECT_F const& area)
	{
		std::map<VIRTUAL_CONTROLLER_BUTTONS, D2D1_RECT_F>::iterator it;
		it = m_touchButtonbinding.find(target);

		if (it != m_touchButtonbinding.end())
		{
			m_touchButtonbinding[target] = area;
		}
		else
		{
			m_touchButtonbinding.emplace(target, area);
		}
	}
	void VirtualController::bindTouchtoTrigger(_In_ VIRTUAL_CONTROLLER_TRIGGERS const& target, _In_ D2D1_RECT_F const& area)
	{
		std::map<VIRTUAL_CONTROLLER_TRIGGERS, D2D1_RECT_F>::iterator it;
		it = m_touchTriggerbinding.find(target);

		if (it != m_touchTriggerbinding.end())
		{
			m_touchTriggerbinding[target] = area;
		}
		else
		{
			m_touchTriggerbinding.emplace(target, area);
		}
	}
	void VirtualController::bindTouchtoThumbstick(_In_ VIRTUAL_CONTROLLER_THUMBSTICKS const& target, _In_ D2D1_POINT_2F const& center, _In_ FLOAT const& radius)
	{
		Virtual_Stick_Touch stick;
		stick.Center = center;
		stick.Radius = radius;

		std::map<VIRTUAL_CONTROLLER_THUMBSTICKS, Virtual_Stick_Touch>::iterator it;
		it = m_touchStickbinding.find(target);

		if (it != m_touchStickbinding.end())
		{
			m_touchStickbinding.at(target) = stick;
		}
		else
		{
			m_touchStickbinding.emplace(target, stick);
		}
	}

	bool PointerIntersect(_In_ Touch_State const& state, _In_ D2D1_RECT_F const& area)
	{
		WOtech::Pointer_Position position = state.Position;

		if (((position.X >= area.left) && (position.X <= area.right)) &&
			((position.Y >= area.top) && (position.Y <= area.bottom)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	Touch_State TouchIntersect(_In_ winrt::com_array<Touch_State> const& state, _In_ D2D1_RECT_F const& area)
	{
		Touch_State temp{};

		for (UINT i = 0; i != state.size(); i++)
		{
			if (state.at(i).PointerID != 0U)
			{
				if (PointerIntersect(state.at(i), area))
					return state.at(i);
			}
		}

		return temp;
	}
	bool isIntersecting(Touch_State state)
	{
		if (state.PointerID != 0U)
			return true;
		else
			return false;
	}

	VirtualController::~VirtualController()
	{
		// Keyboard
		m_keyboardButtonbinding.clear();
		m_keyboardTriggerbinding.clear();
		m_keyboardStickbinding.clear();

		// Mouse
		m_mouseKeybinding.clear();
		m_mouseButtonbinding.clear();

		// Touch
		m_touchButtonbinding.clear();
		m_touchTriggerbinding.clear();
		m_touchStickbinding.clear();
	}

	void VirtualController::Update()
	{
		switch (m_currentInput)
		{
		case CURRENT_INPUT_DEVICE::CURRENT_INPUT_DEVICE_GAMEPAD:
			UpdateGamepad();
			break;
		case CURRENT_INPUT_DEVICE::CURRENT_INPUT_DEVICE_KEYBOARDANDMOUSE:
			UpdateKeyboard();
			UpdateMouse();
			break;
		case CURRENT_INPUT_DEVICE::CURRENT_INPUT_DEVICE_TOUCH:
			UpdateTouch();
			break;
		case CURRENT_INPUT_DEVICE::CURRENT_INPUT_DEVICE_PEN:
			UpdatePen();
			break;
		default:
			break;
		}
	}

	void VirtualController::UpdateGamepad()
	{
		WOtech::Gamepad_State state = m_inputManager->GamepadState(m_currentGamepad);

		// Connection status
		m_state.isConnected = state.Connected;
		m_state.isWireless = state.isWireless;

		// Batterie Percentage
		m_state.ChargePercentage = state.ChargePercentage;;

		// Buttons A-Y
		m_state.Button_A = state.Buttons.A;
		m_state.Button_B = state.Buttons.B;
		m_state.Button_X = state.Buttons.X;
		m_state.Button_Y = state.Buttons.Y;

		// Buttons Menu, View
		m_state.Button_Menu = state.Buttons.Menu;
		m_state.Button_View = state.Buttons.View;

		// Buttons Shoulder
		m_state.Button_LeftShoulder = state.Buttons.LeftShoulder;
		m_state.Button_RightShoulder = state.Buttons.RightShoulder;

		// Buttons Sticks
		m_state.Button_LeftStick = state.Buttons.LeftStick;
		m_state.Button_RightStick = state.Buttons.RightStick;

		// Buttons DPad
		m_state.DPad_Up = state.DPad.Up;
		m_state.DPad_Down = state.DPad.Down;
		m_state.DPad_Left = state.DPad.Left;
		m_state.DPad_Right = state.DPad.Right;

		// Trigger states
		m_state.Trigger_Left = state.Triggers.Left;
		m_state.Trigger_Right = state.Triggers.Right;

		// Tumbstick states
		m_state.Tumbstick_LeftX = state.Tumbsticks.LeftX;
		m_state.Tumbstick_LeftY = state.Tumbsticks.LeftY;
		m_state.Tumbstick_RightX = state.Tumbsticks.RightX;
		m_state.Tumbstick_RightY = state.Tumbsticks.RightY;
	}
	void VirtualController::UpdateKeyboard()
	{
		m_state.isConnected = true;
		m_state.isWireless = false;
		m_state.ChargePercentage = 100U;

		WOtech::Keyboard_State keyState = m_inputManager->getKeyboardState();

		// Buttons
		for (std::map<VIRTUAL_CONTROLLER_BUTTONS, winrt::Windows::System::VirtualKey>::iterator it = m_keyboardButtonbinding.begin(); it != m_keyboardButtonbinding.end(); ++it)
		{
			// Buttons A-Y
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_A)
				m_state.Button_A = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_B)
				m_state.Button_B = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_X)
				m_state.Button_X = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_Y)
				m_state.Button_Y = m_inputManager->KeyDown(it->second);

			// Buttons Menu, View
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_MENU)
				m_state.Button_Menu = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_VIEW)
				m_state.Button_View = m_inputManager->KeyDown(it->second);

			// Buttons Shoulder
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_LEFTSHOULDER)
				m_state.Button_LeftShoulder = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_RIGHTSHOULDER)
				m_state.Button_RightShoulder = m_inputManager->KeyDown(it->second);

			// Buttons Sticks
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_LEFTSTICK)
				m_state.Button_LeftStick = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_RIGHTSTICK)
				m_state.Button_RightStick = m_inputManager->KeyDown(it->second);

			// Buttons DPad
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_UP)
				m_state.DPad_Up = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_DOWN)
				m_state.DPad_Down = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_LEFT)
				m_state.DPad_Left = m_inputManager->KeyDown(it->second);
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_RIGHT)
				m_state.DPad_Right = m_inputManager->KeyDown(it->second);

			// Trigger states
			// Tumbstick states
		}

		// Triggers
		for (std::map<VIRTUAL_CONTROLLER_TRIGGERS, winrt::Windows::System::VirtualKey>::iterator it_t = m_keyboardTriggerbinding.begin(); it_t != m_keyboardTriggerbinding.end(); ++it_t)
		{
			if (it_t->first == VIRTUAL_CONTROLLER_TRIGGERS::VIRTUAL_CONTROLLER_TRIGGERS_LEFT)
			{
				if (m_inputManager->KeyDown(it_t->second))
					m_state.Trigger_Left = 1.0;
				else
					m_state.Trigger_Left = 0.0;
			}
			else
			{
				if (m_inputManager->KeyDown(it_t->second))
					m_state.Trigger_Right = 1.0;
				else
					m_state.Trigger_Right = 0.0;
			}
		}

		// Thumbsticks
		for (std::map<VIRTUAL_CONTROLLER_THUMBSTICKS, Virtual_Stick_Keyboard>::iterator it_s = m_keyboardStickbinding.begin(); it_s != m_keyboardStickbinding.end(); ++it_s)
		{
			Virtual_Stick_Keyboard keyboard = it_s->second;

			bool key_up = m_inputManager->KeyDown(keyboard.Up);
			bool key_down = m_inputManager->KeyDown(keyboard.Down);
			bool key_left = m_inputManager->KeyDown(keyboard.Left);
			bool key_right = m_inputManager->KeyDown(keyboard.Right);

			if (it_s->first == VIRTUAL_CONTROLLER_THUMBSTICKS::VIRTUAL_CONTROLLER_THUMBSTICKS_LEFT)
			{
				// Vertical
				if (key_up && !key_down)
					m_state.Tumbstick_LeftY = 1.0;
				else if (!key_up && key_down)
					m_state.Tumbstick_LeftY = -1.0;
				else
					m_state.Tumbstick_LeftY = 0.0;

				// Horizontal
				if (key_right && !key_left)
					m_state.Tumbstick_LeftX = 1.0;
				else if (!key_right && key_left)
					m_state.Tumbstick_LeftX = -1.0;
				else
					m_state.Tumbstick_LeftX = 0.0;
			}
			else
			{
				// Vertical
				if (key_up && !key_down)
					m_state.Tumbstick_RightY = 1.0;
				else if (!key_up && key_down)
					m_state.Tumbstick_RightY = -1.0;
				else
					m_state.Tumbstick_RightY = 0.0;

				// Horizontal
				if (key_right && !key_left)
					m_state.Tumbstick_RightX = 1.0;
				else if (!key_right && key_left)
					m_state.Tumbstick_RightX = -1.0;
				else
					m_state.Tumbstick_RightX = 0.0;
			}
		}
	}
	void VirtualController::UpdateMouse()
	{
		m_state.isConnected = true;
		m_state.isWireless = false;
		m_state.ChargePercentage = 100U;
		// TODO: add things
	}
	void VirtualController::UpdateTouch()
	{
		winrt::com_array<Touch_State> touchstate = m_inputManager->getTouchState();

		m_state.isConnected = true;
		m_state.isWireless = false;
		m_state.ChargePercentage = 100U;

		// Button states
		for (std::map<VIRTUAL_CONTROLLER_BUTTONS, D2D1_RECT_F>::iterator it = m_touchButtonbinding.begin(); it != m_touchButtonbinding.end(); ++it)
		{
			// Buttons A-Y
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_A)
				m_state.Button_A = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_B)
				m_state.Button_B = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_X)
				m_state.Button_X = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_Y)
				m_state.Button_Y = isIntersecting(TouchIntersect(touchstate, it->second));

			// Buttons Menu, View
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_MENU)
				m_state.Button_Menu = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_VIEW)
				m_state.Button_View = isIntersecting(TouchIntersect(touchstate, it->second));

			// Buttons Shoulder
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_LEFTSHOULDER)
				m_state.Button_LeftShoulder = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_RIGHTSHOULDER)
				m_state.Button_RightShoulder = isIntersecting(TouchIntersect(touchstate, it->second));

			// Buttons Sticks
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_LEFTSTICK)
				m_state.Button_LeftStick = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_RIGHTSTICK)
				m_state.Button_RightStick = isIntersecting(TouchIntersect(touchstate, it->second));

			// Buttons DPad
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_UP)
				m_state.DPad_Up = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_DOWN)
				m_state.DPad_Down = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_LEFT)
				m_state.DPad_Left = isIntersecting(TouchIntersect(touchstate, it->second));
			if (it->first == VIRTUAL_CONTROLLER_BUTTONS::VIRTUAL_CONTROLLER_BUTTONS_DPAD_RIGHT)
				m_state.DPad_Right = isIntersecting(TouchIntersect(touchstate, it->second));
		}
		// Trigger states
		for (std::map<VIRTUAL_CONTROLLER_TRIGGERS, D2D1_RECT_F>::iterator it_t = m_touchTriggerbinding.begin(); it_t != m_touchTriggerbinding.end(); ++it_t)
		{
			Touch_State state;
			if (it_t->first == VIRTUAL_CONTROLLER_TRIGGERS::VIRTUAL_CONTROLLER_TRIGGERS_LEFT)
			{
				state = TouchIntersect(touchstate, it_t->second);
				if (state.PointerID != 0U)
				{
					if (m_currentTriggerLeftID == 0U)
					{
						m_currentTriggerLeftID = state.PointerID;
					}
					if (m_currentTriggerLeftID == state.PointerID)
					{
						// betwen 0.0 and 1.0
						m_state.Trigger_Left = state.Position.Y / (it_t->second.bottom - it_t->second.top);
						if (m_state.Trigger_Left > 1.0)
							m_state.Trigger_Left = 1.0;

						// TODO : Invert or not
						m_state.Trigger_Left = 1 - m_state.Trigger_Left;
					}
				}
				else
				{
					m_currentTriggerLeftID = 0U;
					m_state.Trigger_Left = 0.0;
				}
			}
			else
			{
				state = TouchIntersect(touchstate, it_t->second);
				if (state.PointerID != 0U)
				{
					if (m_currentTriggerRightID == 0U)
					{
						m_currentTriggerRightID = state.PointerID;
					}
					if (m_currentTriggerRightID == state.PointerID)
					{
						// betwen 0.0 and 1.0
						m_state.Trigger_Right = (it_t->second.bottom - it_t->second.top) / state.Position.Y;
						if (m_state.Trigger_Right > 1.0)
							m_state.Trigger_Right = 1.0;

						// TODO : Invert or not
						m_state.Trigger_Right = 1 - m_state.Trigger_Right;
					}
				}
				else
				{
					m_currentTriggerRightID = 0U;
					m_state.Trigger_Right = 0.0;
				}
			}
		}
		// Stick states
		for (std::map<VIRTUAL_CONTROLLER_THUMBSTICKS, Virtual_Stick_Touch>::iterator it_s = m_touchStickbinding.begin(); it_s != m_touchStickbinding.end(); ++it_s)
		{
			// TODO: Circle insteed of RECT
			Virtual_Stick_Touch stick = it_s->second;
			D2D1_RECT_F rect{ stick.Center.x - stick.Radius, stick.Center.y - stick.Radius, stick.Center.x + stick.Radius, stick.Center.y + stick.Radius };

			Touch_State state;
			if (it_s->first == VIRTUAL_CONTROLLER_THUMBSTICKS::VIRTUAL_CONTROLLER_THUMBSTICKS_LEFT)
			{
				state = TouchIntersect(touchstate, rect);
				if (state.PointerID != 0U)
				{
					if (m_currentStickLeftID == 0U)
					{
						m_currentStickLeftID = state.PointerID;
					}
					if (m_currentStickLeftID == state.PointerID)
					{
						// betwen -1.0 and 1.0
						m_state.Tumbstick_LeftX = (rect.right - rect.left) / state.Position.X;
						if (state.Position.X < (rect.left + ((rect.right - rect.left) / 2)))
							m_state.Tumbstick_LeftX *= -1;
						m_state.Tumbstick_LeftY = (rect.bottom - rect.top) / state.Position.Y;
						if (state.Position.Y > (rect.top + ((rect.bottom - rect.top) / 2)))
							m_state.Tumbstick_LeftY *= -1;
					}
				}
				else
				{
					m_currentStickLeftID = 0U;
					m_state.Tumbstick_LeftX = 0.0;
					m_state.Tumbstick_LeftY = 0.0;
				}
			}
			else
			{
				state = TouchIntersect(touchstate, rect);
				if (state.PointerID != 0U)
				{
					if (m_currentStickLeftID == 0U)
					{
						m_currentStickLeftID = state.PointerID;
					}
					if (m_currentStickLeftID == state.PointerID)
					{
						// betwen -1.0 and 1.0
						m_state.Tumbstick_RightX = (rect.right - rect.left) / state.Position.X;
						if (state.Position.X < (rect.left + ((rect.right - rect.left) / 2)))
							m_state.Tumbstick_RightX *= -1;
						m_state.Tumbstick_RightY = (rect.bottom - rect.top) / state.Position.Y;
						if (state.Position.Y > (rect.top + ((rect.bottom - rect.top) / 2)))
							m_state.Tumbstick_RightY *= -1;
					}
				}
				else
				{
					m_currentStickLeftID = 0U;
					m_state.Tumbstick_LeftX = 0.0;
					m_state.Tumbstick_LeftY = 0.0;
				}
			}
		}
	}
	void VirtualController::UpdatePen()
	{
		m_state.isConnected = true;
		m_state.isWireless = false;
		m_state.ChargePercentage = 100U;
		// TODO: add things
	}
}