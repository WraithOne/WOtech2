////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Input.h
///
///			Description:
///
///			Created:	01.05.2014
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_INPUT_H
#define WO_INPUT_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"

namespace WOtech
{
	class Window;

	enum VIRTUALKEY_MOUSE
	{
		VIRTUALKEY_MOUSE_LEFTBUTTON,
		VIRTUALKEY_MOUSE_RIGHTBUTTON,
		VIRTUALKEY_MOUSE_MIDDLEBUTTON,
		VIRTUALKEY_MOUSE_X1BUTTON,
		VIRTUALKEY_MOUSE_X2BUTTON
	};

	struct Touch_Capabilities
	{
		bool	isPresent;
		UINT	MinimumContacts;
	};

	struct Pen_Capabilities
	{
		bool												isPresent;
		winrt::Windows::Devices::Input::PointerDeviceType	DeviceType;
		bool												isIntegrated;
		UINT												MaximumContacts;
		D2D1_RECT_F											PhysicalRect;
		D2D1_RECT_F											ScreenRect;
	};

	struct Mouse_Capabilities
	{
		bool	isPresent;
		bool	isVerticalWheelPresent;
		bool	isHorizontalWheelPresent;
		bool	SwapButtons;
		UINT	NumberofButtons;
	};

	struct Keyboard_Capabilities
	{
		bool isPresent;
	};

	struct Keyboard_State
	{
		bool Back;					// VK_BACK, 0x8
		bool Tab;					// VK_TAB, 0x9
		bool Enter;				// VK_RETURN, 0xD
		bool Pause;				// VK_PAUSE, 0x13
		bool CapsLock;				// VK_CAPITAL, 0x14
		bool Kana;					// VK_KANA, 0x15
		bool Kanji;				// VK_KANJI, 0x19
		bool Escape;				// VK_ESCAPE, 0x1B
		bool ImeConvert;			// VK_CONVERT, 0x1C
		bool ImeNoConvert;			// VK_NONCONVERT, 0x1D
		bool Space;				// VK_SPACE, 0x20
		bool PageUp;				// VK_PRIOR, 0x21
		bool PageDown;				// VK_NEXT, 0x22
		bool End;					// VK_END, 0x23
		bool Home;					// VK_HOME, 0x24
		bool Left;					// VK_LEFT, 0x25
		bool Up;					// VK_UP, 0x26
		bool Right;				// VK_RIGHT, 0x27
		bool Down;					// VK_DOWN, 0x28
		bool Select;				// VK_SELECT, 0x29
		bool Print;				// VK_PRINT, 0x2A
		bool Execute;				// VK_EXECUTE, 0x2B
		bool PrintScreen;			// VK_SNAPSHOT, 0x2C
		bool Insert;				// VK_INSERT, 0x2D
		bool Delete;				// VK_DELETE, 0x2E
		bool Help;					// VK_HELP, 0x2F
		bool D0;					// 0x30
		bool D1;					// 0x31
		bool D2;					// 0x32
		bool D3;					// 0x33
		bool D4;					// 0x34
		bool D5;					// 0x35
		bool D6;					// 0x36
		bool D7;					// 0x37
		bool D8;					// 0x38
		bool D9;					// 0x39
		bool A;					// 0x41
		bool B;					// 0x42
		bool C;					// 0x43
		bool D;					// 0x44
		bool E;					// 0x45
		bool F;					// 0x46
		bool G;					// 0x47
		bool H;					// 0x48
		bool I;					// 0x49
		bool J;					// 0x4A
		bool K;					// 0x4B
		bool L;					// 0x4C
		bool M;					// 0x4D
		bool N;					// 0x4E
		bool O;					// 0x4F
		bool P;					// 0x50
		bool Q;					// 0x51
		bool R;					// 0x52
		bool S;					// 0x53
		bool T;					// 0x54
		bool U;					// 0x55
		bool V;					// 0x56
		bool W;					// 0x57
		bool X;					// 0x58
		bool Y;					// 0x59
		bool Z;					// 0x5A
		bool LeftWindows;			// VK_LWIN, 0x5B
		bool RightWindows;			// VK_RWIN, 0x5C
		bool Apps;					// VK_APPS, 0x5D
		bool Sleep;				// VK_SLEEP, 0x5F
		bool NumPad0;				// VK_NUMPAD0, 0x60
		bool NumPad1;				// VK_NUMPAD1, 0x61
		bool NumPad2;				// VK_NUMPAD2, 0x62
		bool NumPad3;				// VK_NUMPAD3, 0x63
		bool NumPad4;				// VK_NUMPAD4, 0x64
		bool NumPad5;				// VK_NUMPAD5, 0x65
		bool NumPad6;				// VK_NUMPAD6, 0x66
		bool NumPad7;				// VK_NUMPAD7, 0x67
		bool NumPad8;				// VK_NUMPAD8, 0x68
		bool NumPad9;				// VK_NUMPAD9, 0x69
		bool Multiply;				// VK_MULTIPLY, 0x6A
		bool Add;					// VK_ADD, 0x6B
		bool Separator;			// VK_SEPARATOR, 0x6C
		bool Subtract;				// VK_SUBTRACT, 0x6D
		bool Decimal;				// VK_DECIMANL, 0x6E
		bool Divide;				// VK_DIVIDE, 0x6F
		bool F1;					// VK_F1, 0x70
		bool F2;					// VK_F2, 0x71
		bool F3;					// VK_F3, 0x72
		bool F4;					// VK_F4, 0x73
		bool F5;					// VK_F5, 0x74
		bool F6;					// VK_F6, 0x75
		bool F7;					// VK_F7, 0x76
		bool F8;					// VK_F8, 0x77
		bool F9;					// VK_F9, 0x78
		bool F10;					// VK_F10, 0x79
		bool F11;					// VK_F11, 0x7A
		bool F12;					// VK_F12, 0x7B
		bool F13;					// VK_F13, 0x7C
		bool F14;					// VK_F14, 0x7D
		bool F15;					// VK_F15, 0x7E
		bool F16;					// VK_F16, 0x7F
		bool F17;					// VK_F17, 0x80
		bool F18;					// VK_F18, 0x81
		bool F19;					// VK_F19, 0x82
		bool F20;					// VK_F20, 0x83
		bool F21;					// VK_F21, 0x84
		bool F22;					// VK_F22, 0x85
		bool F23;					// VK_F23, 0x86
		bool F24;					// VK_F24, 0x87
		bool NumLock;				// VK_NUMLOCK, 0x90
		bool Scroll;				// VK_SCROLL, 0x91
		bool LeftShift;			// VK_LSHIFT, 0xA0
		bool RightShift;			// VK_RSHIFT, 0xA1
		bool LeftControl;			// VK_LCONTROL, 0xA2
		bool RightControl;			// VK_RCONTROL, 0xA3
		bool LeftAlt;				// VK_LMENU, 0xA4
		bool RightAlt;				// VK_RMENU, 0xA5
		bool BrowserBack;			// VK_BROWSER_BACK, 0xA6
		bool BrowserForward;		// VK_BROWSER_FORWARD, 0xA7
		bool BrowserRefresh;		// VK_BROWSER_REFRESH, 0xA8
		bool BrowserStop;			// VK_BROWSER_STOP, 0xA9
		bool BrowserSearch;		// VK_BROWSER_SEARCH, 0xAA
		bool BrowserFavorites;		// VK_BROWSER_FAVORITES, 0xAB
		bool BrowserHome;			// VK_BROWSER_HOME, 0xAC
		bool VolumeMute;			// VK_VOLUME_MUTE, 0xAD
		bool VolumeDown;			// VK_VOLUME_DOWN, 0xAE
		bool VolumeUp;				// VK_VOLUME_UP, 0xAF
		bool MediaNextTrack;		// VK_MEDIA_NEXT_TRACK, 0xB0
		bool MediaPreviousTrack;	// VK_MEDIA_PREV_TRACK, 0xB1
		bool MediaStop;			// VK_MEDIA_STOP, 0xB2
		bool MediaPlayPause;		// VK_MEDIA_PLAY_PAUSE, 0xB3
		bool LaunchMail;			// VK_LAUNCH_MAIL, 0xB4
		bool SelectMedia;			// VK_LAUNCH_MEDIA_SELECT, 0xB5
		bool LaunchApplication1;	// VK_LAUNCH_APP1, 0xB6
		bool LaunchApplication2;	// VK_LAUNCH_APP2, 0xB7
		bool OemSemicolon;			// VK_OEM_1, 0xBA
		bool OemPlus;				// VK_OEM_PLUS, 0xBB
		bool OemComma;				// VK_OEM_COMMA, 0xBC
		bool OemMinus;				// VK_OEM_MINUS, 0xBD
		bool OemPeriod;			// VK_OEM_PERIOD, 0xBE
		bool OemQuestion;			// VK_OEM_2, 0xBF
		bool OemTilde;				// VK_OEM_3, 0xC0
		bool OemOpenBrackets;		// VK_OEM_4, 0xDB
		bool OemPipe;				// VK_OEM_5, 0xDC
		bool OemCloseBrackets;		// VK_OEM_6, 0xDD
		bool OemQuotes;			// VK_OEM_7, 0xDE
		bool Oem8;					// VK_OEM_8, 0xDF
		bool OemBackslash;			// VK_OEM_102, 0xE2
		bool ProcessKey;			// VK_PROCESSKEY, 0xE5
		bool OemCopy;				// 0XF2
		bool OemAuto;				// 0xF3
		bool OemEnlW;				// 0xF4
		bool Attn;					// VK_ATTN, 0xF6
		bool Crsel;				// VK_CRSEL, 0xF7
		bool Exsel;				// VK_EXSEL, 0xF8
		bool EraseEof;				// VK_EREOF, 0xF9
		bool Play;					// VK_PLAY, 0xFA
		bool Zoom;					// VK_ZOOM, 0xFB
		bool Pa1;					// VK_PA1, 0xFD
		bool OemClear;				// VK_OEM_CLEAR, 0xFE
	};

	struct Pointer_Position
	{
		FLOAT X;
		FLOAT Y;
	};

	struct Touch_State
	{
		UINT				PointerID;
		Pointer_Position	Position;
	};

	struct Mouse_Position
	{
		INT X;
		INT Y;
	};

	struct Mouse_Buttons
	{
		bool LeftButton;
		bool RightButton;
		bool MiddleButton;
		bool X1Button;
		bool X2Button;
	};

	struct Mouse_State
	{
		UINT			PointerID;
		Mouse_Position	Position;
		Mouse_Buttons	Buttons;
		INT				WheelDelta;
	};

	struct Pen_State
	{
		UINT				PointerID;
		Pointer_Position	Position;
		bool				BarrelButton;
		bool				isErazer;
		FLOAT				Pressure;
	};

	enum GAMEPAD_INDEX
	{
		GAMEPAD_INDEX_PLAYERONE,
		GAMEPAD_INDEX_PLAYERTWO,
		GAMEPAD_INDEX_PLAYERTHREE,
		GAMEPAD_INDEX_PLAYERFOUR,
		GAMEPAD_INDEX_PLAYERFIVE,
		GAMEPAD_INDEX_PLAYERSIX,
		GAMEPAD_INDEX_PLAYERSEVEN,
		GAMEPAD_INDEX_PLAYEREIGHT
	};

	struct Gamepad_Buttons
	{
		bool A;
		bool B;
		bool X;
		bool Y;
		bool LeftStick;
		bool RightStick;
		bool LeftShoulder;
		bool RightShoulder;
		bool View;
		bool Menu;
	};

	struct Gamepad_DPad
	{
		bool Up;
		bool Down;
		bool Left;
		bool Right;
	};

	struct Gamepad_Trigger_State
	{
		DOUBLE Left;
		DOUBLE Right;
	};

	struct Gamepad_Tumbstick_State
	{
		DOUBLE LeftX;
		DOUBLE LeftY;
		DOUBLE RightX;
		DOUBLE RightY;
	};

	struct Gamepad_State
	{
		bool					Connected;
		bool					isWireless;
		UINT					ChargePercentage;
		UINT64					TimeStamp;
		Gamepad_Buttons			Buttons;
		Gamepad_DPad			DPad;
		Gamepad_Tumbstick_State	Tumbsticks;
		Gamepad_Trigger_State	Triggers;
	};

	class InputManager
	{
	public:
		InputManager() = delete;
		InputManager(_In_ WOtech::Window* const& window);
		~InputManager();

		void SuspendInput();
		void ResumeInput();

		// Keyboard
		bool KeyboardConnected();
		Keyboard_State getKeyboardState();
		bool KeyDown(_In_ winrt::Windows::System::VirtualKey const& key);

		// Touch
		bool TouchConnected();
		winrt::com_array<WOtech::Touch_State> getTouchState();
		// Pointer
		bool PenConnected();
		winrt::com_array<WOtech::Pen_State> getPenState();

		// Mouse
		bool MouseConnected();
		Mouse_State getMouseState();
		void MouseShowCursor(_In_ bool const& Show);
		bool MouseCursorVisible();

		// Gamepad
		bool GamepadConnected(_In_ WOtech::GAMEPAD_INDEX const& PlayerIndex);
		Gamepad_State GamepadState(_In_ WOtech::GAMEPAD_INDEX const& PlayerIndex);
		void GamepadSetVibration(_In_ WOtech::GAMEPAD_INDEX const& PlayerIndex, _In_ winrt::Windows::Gaming::Input::GamepadVibration const& Vibration);

		// OrientationSensor
		bool OrientationSensorConnected();
		void ActivateOrientationSensor(_In_ winrt::Windows::Devices::Sensors::SensorReadingType const& Type, _In_ winrt::Windows::Devices::Sensors::SensorOptimizationGoal const& Goal, _In_ UINT const& Interval);
		void DeactivateOrientationSensor();
		bool OrientationSensorActive();
		winrt::Windows::Devices::Sensors::SensorQuaternion OrientationSensorQuaternion(void);
		winrt::Windows::Devices::Sensors::SensorRotationMatrix OrientationSensorMatrix(void);

	private:
		void Initialize();
		void ScanDeviceCapabilities();
		void ScanGamePad();

		// Pointer
		void OnPointerPressed(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnPointerMoved(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnPointerReleased(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnPointerEntered(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnPointerExited(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnPointerWheelChanged(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args);
		void OnMouseMoved(_In_ winrt::Windows::Devices::Input::MouseDevice const& MouseDevice, _In_ winrt::Windows::Devices::Input::MouseEventArgs const& Args);

		void UpdatePointerDevices(_In_ winrt::Windows::UI::Input::PointerPoint const& Device);
		void RemovePointerDevice(_In_ winrt::Windows::UI::Input::PointerPoint const& Device);

		// Keyboard
		void OnKeyDown(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::KeyEventArgs const& Args);
		void OnKeyUp(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::KeyEventArgs const& Args);

		// Orientation Sensor
		void ReadingChanged(_In_ winrt::Windows::Devices::Sensors::OrientationSensor const& Sender, _In_ winrt::Windows::Devices::Sensors::OrientationSensorReadingChangedEventArgs const& Args);

		// Gamepad
		void OnGamepadAdded(_In_ winrt::Windows::Foundation::IInspectable const& Sender, _In_ winrt::Windows::Gaming::Input::Gamepad const& Gamepad);
		void OnGamepadRemoved(_In_ winrt::Windows::Foundation::IInspectable const& Sender, _In_ winrt::Windows::Gaming::Input::Gamepad const& Gamepad);

	private:
		WOtech::Window*												m_window;

		bool														m_isInitialized;

		// Keyboard
		winrt::Windows::Devices::Input::KeyboardCapabilities		m_keyBoardCapabilities;
		WOtech::Keyboard_State										m_keyboardState;

		// Touch
		winrt::Windows::Devices::Input::TouchCapabilities			m_touchCapabilities;
		// Mouse
		winrt::Windows::Devices::Input::MouseCapabilities			m_mouseCapabilities;
		WOtech::Mouse_Position										m_mouseDelta;
		// Pointer a.k.a. Touch/Pen/Mouse
		static const UINT											MAX_POINTER_COUNT = 50;// TODO: Find proper max count
		winrt::com_array<winrt::Windows::UI::Input::PointerPoint>	m_pointerdevices[MAX_POINTER_COUNT];

		// Orientation Sensor
		winrt::Windows::Devices::Sensors::OrientationSensor			m_orientationSensor;
		winrt::event_token											m_orientationToken;
		winrt::Windows::Devices::Sensors::OrientationSensorReading	m_orientationSensorReading;
		bool														m_orientationActive;

		// Gamepad
		static const UINT											MAX_PLAYER_COUNT = 8;
		winrt::com_array<winrt::Windows::Gaming::Input::Gamepad>	m_gamePad[MAX_PLAYER_COUNT];
	};
}// namespace WOtech

#endif
