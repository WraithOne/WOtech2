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
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_VIRTUALCONTROLLER_H
#define WO_VIRTUALCONTROLLER_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"

namespace WOtech
{
	enum CURRENT_INPUT_DEVICE
	{
		CURRENT_INPUT_DEVICE_UNSPECIFIED,
		CURRENT_INPUT_DEVICE_GAMEPAD,
		CURRENT_INPUT_DEVICE_KEYBOARDANDMOUSE,
		CURRENT_INPUT_DEVICE_TOUCH,
		CURRENT_INPUT_DEVICE_PEN,
	};
	enum VIRTUAL_CONTROLLER_BUTTONS
	{
		VIRTUAL_CONTROLLER_BUTTONS_A,
		VIRTUAL_CONTROLLER_BUTTONS_B,
		VIRTUAL_CONTROLLER_BUTTONS_X,
		VIRTUAL_CONTROLLER_BUTTONS_Y,
		VIRTUAL_CONTROLLER_BUTTONS_LEFTSTICK,
		VIRTUAL_CONTROLLER_BUTTONS_RIGHTSTICK,
		VIRTUAL_CONTROLLER_BUTTONS_LEFTSHOULDER,
		VIRTUAL_CONTROLLER_BUTTONS_RIGHTSHOULDER,
		VIRTUAL_CONTROLLER_BUTTONS_VIEW,
		VIRTUAL_CONTROLLER_BUTTONS_MENU,
		VIRTUAL_CONTROLLER_BUTTONS_DPAD_UP,
		VIRTUAL_CONTROLLER_BUTTONS_DPAD_DOWN,
		VIRTUAL_CONTROLLER_BUTTONS_DPAD_LEFT,
		VIRTUAL_CONTROLLER_BUTTONS_DPAD_RIGHT
	};
	enum VIRTUAL_CONTROLLER_TRIGGERS
	{
		VIRTUAL_CONTROLLER_TRIGGERS_LEFT,
		VIRTUAL_CONTROLLER_TRIGGERS_RIGHT
	};
	enum VIRTUAL_CONTROLLER_THUMBSTICKS
	{
		VIRTUAL_CONTROLLER_THUMBSTICKS_LEFT,
		VIRTUAL_CONTROLLER_THUMBSTICKS_
	};

	struct Virtual_Controller_State
	{
		bool	isConnected;
		bool	isWireless;
		UINT	ChargePercentage;

		bool	Button_A;
		bool	Button_B;
		bool	Button_X;
		bool	Button_Y;
		bool	Button_LeftStick;
		bool	Button_RightStick;
		bool	Button_LeftShoulder;
		bool	Button_RightShoulder;
		bool	Button_View;
		bool	Button_Menu;

		bool	DPad_Up;
		bool	DPad_Down;
		bool	DPad_Left;
		bool	DPad_Right;

		double	Trigger_Left;
		double	Trigger_Right;

		double	Tumbstick_LeftX;
		double	Tumbstick_LeftY;
		double	Tumbstick_RightX;
		double	Tumbstick_RightY;
	};

	struct Virtual_Stick_Touch
	{
		D2D1_POINT_2F Center;
		FLOAT Radius;
	};

	struct Virtual_Stick_Keyboard
	{
		winrt::Windows::System::VirtualKey Up;
		winrt::Windows::System::VirtualKey Down;
		winrt::Windows::System::VirtualKey Left;
		winrt::Windows::System::VirtualKey Right;
	};

	class VirtualController
	{
	public:
		VirtualController(_In_ WOtech::InputManager* const& input);
		~VirtualController();

		void setCurrentInput(_In_ WOtech::CURRENT_INPUT_DEVICE const& current);
		WOtech::CURRENT_INPUT_DEVICE getCurrent();
		WOtech::Virtual_Controller_State getState();

		// Gamepad
		void bindGamepad(_In_ WOtech::GAMEPAD_INDEX const& number);

		// Keyboard and Mice
		void bindKeyboardtoButton(_In_ WOtech::VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ winrt::Windows::System::VirtualKey const& key);
		bool getKeyboardButtonBinding(_In_ WOtech::VIRTUAL_CONTROLLER_BUTTONS const& target, _Out_ winrt::Windows::System::VirtualKey* key) const;
		void bindKeyboardtoTrigger(_In_ WOtech::VIRTUAL_CONTROLLER_TRIGGERS const& target, _In_ winrt::Windows::System::VirtualKey const& key);
		void bindKeyboardtoThumbstick(_In_ WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS const& target, _In_ winrt::Windows::System::VirtualKey const& keyUP, _In_ winrt::Windows::System::VirtualKey const& keyDOWN, _In_ winrt::Windows::System::VirtualKey const& keyLEFT, _In_ winrt::Windows::System::VirtualKey const& keyRIGHT);

		void bindMousetoButton(_In_ WOtech::VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ D2D1_RECT_F const& area);
		void bindMouseKeytoButton(_In_ WOtech::VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ WOtech::VIRTUALKEY_MOUSE const& key);
		void bindMouseWheeltoTrigger(_In_ WOtech::VIRTUAL_CONTROLLER_TRIGGERS const& target);
		void bindMousetoThumbstick(_In_ WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS const& target);

		// Touch and Pen
		void bindTouchtoButton(_In_ WOtech::VIRTUAL_CONTROLLER_BUTTONS const& target, _In_ D2D1_RECT_F const& area);
		void bindTouchtoTrigger(_In_ WOtech::VIRTUAL_CONTROLLER_TRIGGERS const& target, _In_ D2D1_RECT_F const& Area);
		void bindTouchtoThumbstick(_In_ WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS const& target, _In_ D2D1_POINT_2F const& center, _In_ FLOAT const& radius);

	private:
		void Update();

		void UpdateGamepad();
		void UpdateKeyboard();
		void UpdateMouse();
		void UpdateTouch();
		void UpdatePen();

	private:
		WOtech::InputManager*																m_inputManager;

		WOtech::Virtual_Controller_State													m_state;

		WOtech::CURRENT_INPUT_DEVICE														m_currentInput;
		WOtech::GAMEPAD_INDEX																m_currentGamepad;

		std::map<WOtech::VIRTUAL_CONTROLLER_BUTTONS, winrt::Windows::System::VirtualKey>	m_keyboardButtonbinding;
		std::map<WOtech::VIRTUAL_CONTROLLER_TRIGGERS, winrt::Windows::System::VirtualKey>	m_keyboardTriggerbinding;
		std::map<WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS, WOtech::Virtual_Stick_Keyboard>	m_keyboardStickbinding;

		std::map<WOtech::VIRTUAL_CONTROLLER_BUTTONS, WOtech::VIRTUALKEY_MOUSE>				m_mouseKeybinding;
		std::map<WOtech::VIRTUAL_CONTROLLER_BUTTONS, D2D1_RECT_F>							m_mouseButtonbinding;
		WOtech::VIRTUAL_CONTROLLER_TRIGGERS													m_mouseWheelbinding;
		WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS												m_mousebinding;

		std::map<WOtech::VIRTUAL_CONTROLLER_BUTTONS, D2D1_RECT_F>							m_touchButtonbinding;
		std::map<WOtech::VIRTUAL_CONTROLLER_TRIGGERS, D2D1_RECT_F>							m_touchTriggerbinding;
		UINT																				m_currentTriggerLeftID;
		UINT																				m_currentTriggerRightID;

		std::map<WOtech::VIRTUAL_CONTROLLER_THUMBSTICKS, WOtech::Virtual_Stick_Touch>		m_touchStickbinding;
		UINT																				m_currentStickLeftID;
		UINT																				m_currentStickRightID;
	};
};
#endif
