////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Input.cpp
///
///			Description:
///
///			Created:	02.05.2014
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
	InputManager::InputManager(_In_ WOtech::Window* const& window) :
		m_window(window),
		m_isInitialized(false),
		m_keyBoardCapabilities(),
		m_keyboardState(),
		m_touchCapabilities(),
		m_mouseCapabilities(),
		m_mouseDelta(),
		m_pointerdevices(),
		m_orientationSensor(nullptr),
		m_orientationToken(),
		m_orientationSensorReading(nullptr),
		m_orientationActive(false)
	{
		Initialize();
	}

	void InputManager::SuspendInput()
	{
		for (UINT i = 0; i != MAX_PLAYER_COUNT; i++)
		{
			m_gamePad->at(i) = nullptr;
		}
	}

	void InputManager::ResumeInput()
	{
		ScanGamePad();
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::Initialize()
	{
		m_isInitialized = false;
		if (m_window == nullptr || !m_window->isValid())
		{
			return;
		}
		auto window = m_window->getCoreWindow();

		// opt in to receive key events
		window.KeyDown({ this, &WOtech::InputManager::OnKeyDown });
		window.KeyUp({ this, &WOtech::InputManager::OnKeyUp });

		// opt in to receive touch/mouse events
		window.PointerPressed({ this, &WOtech::InputManager::OnPointerPressed });
		window.PointerMoved({ this, &WOtech::InputManager::OnPointerMoved });
		window.PointerReleased({ this, &WOtech::InputManager::OnPointerReleased });
		window.PointerEntered({ this, &WOtech::InputManager::OnPointerEntered });
		window.PointerExited({ this, &WOtech::InputManager::OnPointerExited });
		window.PointerWheelChanged({ this, &WOtech::InputManager::OnPointerWheelChanged });

		// There is a separate handler for mouse only relative mouse movement events.
		winrt::Windows::Devices::Input::MouseDevice::GetForCurrentView().MouseMoved({ this, &WOtech::InputManager::OnMouseMoved });

		// Detect game pad connection and disconnection events.
		winrt::Windows::Gaming::Input::Gamepad::GamepadAdded({ this, &WOtech::InputManager::OnGamepadAdded });
		winrt::Windows::Gaming::Input::Gamepad::GamepadRemoved({ this, &WOtech::InputManager::OnGamepadRemoved });

		// Scan for Capabilities
		ScanDeviceCapabilities();

		// Scan for Gamepads
		ScanGamePad();

		m_isInitialized = true;
	}

	void InputManager::ScanDeviceCapabilities()
	{
		m_keyBoardCapabilities = winrt::Windows::Devices::Input::KeyboardCapabilities();
		m_touchCapabilities = winrt::Windows::Devices::Input::TouchCapabilities();
		m_mouseCapabilities = winrt::Windows::Devices::Input::MouseCapabilities();
	}

	/// Events
	void InputManager::ReadingChanged(_In_ winrt::Windows::Devices::Sensors::OrientationSensor const& Sender, _In_ winrt::Windows::Devices::Sensors::OrientationSensorReadingChangedEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		m_orientationSensorReading = Args.Reading();
	}

	inline void HandleKeys(_Inout_ WOtech::Keyboard_State* keystate, _In_ winrt::Windows::System::VirtualKey key, _In_ bool down)
	{
		switch (key)
		{
		case winrt::Windows::System::VirtualKey::None:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::LeftButton:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::RightButton:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::Cancel:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::MiddleButton:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::XButton1:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::XButton2:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::Back:
			keystate->Back = down;
			break;
		case winrt::Windows::System::VirtualKey::Tab:
			keystate->Tab = down;
			break;
		case winrt::Windows::System::VirtualKey::Clear:
			//keystate. = down;
			break;
		case winrt::Windows::System::VirtualKey::Enter:
			keystate->Enter = down;
			break;
		case winrt::Windows::System::VirtualKey::Shift:
			//keystate. = down;
			break;
		case winrt::Windows::System::VirtualKey::Control:
			//keystate. = down;
			break;
		case winrt::Windows::System::VirtualKey::Menu:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::Pause:
			keystate->Pause = down;
			break;
		case winrt::Windows::System::VirtualKey::CapitalLock:
			keystate->CapsLock = down;
			break;
		case winrt::Windows::System::VirtualKey::Kana:
			keystate->Kana = down;
			break;
			//case Windows::System::VirtualKey::Hangul:
				//keystate-> = down;
				//	break;
		case winrt::Windows::System::VirtualKey::Junja:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::Final:
			break;
			//case Windows::System::VirtualKey::Hanja:
				//keystate-> = down;
				//break;
		case winrt::Windows::System::VirtualKey::Kanji:
			keystate->Kanji = down;
			break;
		case winrt::Windows::System::VirtualKey::Escape:
			keystate->Escape = down;
			break;
		case winrt::Windows::System::VirtualKey::Convert:
			keystate->ImeConvert = down;
			break;
		case winrt::Windows::System::VirtualKey::NonConvert:
			keystate->ImeNoConvert = down;
			break;
		case winrt::Windows::System::VirtualKey::Accept:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::ModeChange:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::Space:
			keystate->Space = down;
			break;
		case winrt::Windows::System::VirtualKey::PageUp:
			keystate->PageUp = down;
			break;
		case winrt::Windows::System::VirtualKey::PageDown:
			keystate->PageDown = down;
			break;
		case winrt::Windows::System::VirtualKey::End:
			keystate->End = down;
			break;
		case winrt::Windows::System::VirtualKey::Home:
			keystate->Home = down;
			break;
		case winrt::Windows::System::VirtualKey::Left:
			keystate->Left = down;
			break;
		case winrt::Windows::System::VirtualKey::Up:
			keystate->Up = down;
			break;
		case winrt::Windows::System::VirtualKey::Right:
			keystate->Right = down;
			break;
		case winrt::Windows::System::VirtualKey::Down:
			keystate->Down = down;
			break;
		case winrt::Windows::System::VirtualKey::Select:
			keystate->Select = down;
			break;
		case winrt::Windows::System::VirtualKey::Print:
			keystate->Print = down;
			break;
		case winrt::Windows::System::VirtualKey::Execute:
			keystate->Execute = down;
			break;
		case winrt::Windows::System::VirtualKey::Snapshot:
			keystate->PrintScreen = down;
			break;
		case winrt::Windows::System::VirtualKey::Insert:
			keystate->Insert = down;
			break;
		case winrt::Windows::System::VirtualKey::Delete:
			keystate->Delete = down;
			break;
		case winrt::Windows::System::VirtualKey::Help:
			keystate->Help = down;
			break;
		case winrt::Windows::System::VirtualKey::Number0:
			keystate->D0 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number1:
			keystate->D1 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number2:
			keystate->D2 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number3:
			keystate->D3 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number4:
			keystate->D4 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number5:
			keystate->D5 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number6:
			keystate->D6 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number7:
			keystate->D7 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number8:
			keystate->D8 = down;
			break;
		case winrt::Windows::System::VirtualKey::Number9:
			keystate->D9 = down;
			break;
		case winrt::Windows::System::VirtualKey::A:
			keystate->A = down;
			break;
		case winrt::Windows::System::VirtualKey::B:
			keystate->B = down;
			break;
		case winrt::Windows::System::VirtualKey::C:
			keystate->C = down;
			break;
		case winrt::Windows::System::VirtualKey::D:
			keystate->D = down;
			break;
		case winrt::Windows::System::VirtualKey::E:
			keystate->E = down;
			break;
		case winrt::Windows::System::VirtualKey::F:
			keystate->F = down;
			break;
		case winrt::Windows::System::VirtualKey::G:
			keystate->G = down;
			break;
		case winrt::Windows::System::VirtualKey::H:
			keystate->H = down;
			break;
		case winrt::Windows::System::VirtualKey::I:
			keystate->I = down;
			break;
		case winrt::Windows::System::VirtualKey::J:
			keystate->J = down;
			break;
		case winrt::Windows::System::VirtualKey::K:
			keystate->K = down;
			break;
		case winrt::Windows::System::VirtualKey::L:
			keystate->L = down;
			break;
		case winrt::Windows::System::VirtualKey::M:
			keystate->M = down;
			break;
		case winrt::Windows::System::VirtualKey::N:
			keystate->N = down;
			break;
		case winrt::Windows::System::VirtualKey::O:
			keystate->O = down;
			break;
		case winrt::Windows::System::VirtualKey::P:
			keystate->P = down;
			break;
		case winrt::Windows::System::VirtualKey::Q:
			keystate->Q = down;
			break;
		case winrt::Windows::System::VirtualKey::R:
			keystate->R = down;
			break;
		case winrt::Windows::System::VirtualKey::S:
			keystate->S = down;
			break;
		case winrt::Windows::System::VirtualKey::T:
			keystate->T = down;
			break;
		case winrt::Windows::System::VirtualKey::U:
			keystate->U = down;
			break;
		case winrt::Windows::System::VirtualKey::V:
			keystate->V = down;
			break;
		case winrt::Windows::System::VirtualKey::W:
			keystate->W = down;
			break;
		case winrt::Windows::System::VirtualKey::X:
			keystate->X = down;
			break;
		case winrt::Windows::System::VirtualKey::Y:
			keystate->Y = down;
			break;
		case winrt::Windows::System::VirtualKey::Z:
			keystate->Z = down;
			break;
		case winrt::Windows::System::VirtualKey::LeftWindows:
			keystate->LeftWindows = down;
			break;
		case winrt::Windows::System::VirtualKey::RightWindows:
			keystate->RightWindows = down;
			break;
		case winrt::Windows::System::VirtualKey::Application:
			keystate->Apps = down;
			break;
		case winrt::Windows::System::VirtualKey::Sleep:
			keystate->Sleep = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad0:
			keystate->NumPad0 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad1:
			keystate->NumPad1 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad2:
			keystate->NumPad2 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad3:
			keystate->NumPad3 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad4:
			keystate->NumPad4 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad5:
			keystate->NumPad5 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad6:
			keystate->NumPad6 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad7:
			keystate->NumPad7 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad8:
			keystate->NumPad8 = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad9:
			keystate->NumPad9 = down;
			break;
		case winrt::Windows::System::VirtualKey::Multiply:
			keystate->Multiply = down;
			break;
		case winrt::Windows::System::VirtualKey::Add:
			keystate->Add = down;
			break;
		case winrt::Windows::System::VirtualKey::Separator:
			keystate->Separator = down;
			break;
		case winrt::Windows::System::VirtualKey::Subtract:
			keystate->Subtract = down;
			break;
		case winrt::Windows::System::VirtualKey::Decimal:
			keystate->Decimal = down;
			break;
		case winrt::Windows::System::VirtualKey::Divide:
			keystate->Divide = down;
			break;
		case winrt::Windows::System::VirtualKey::F1:
			keystate->F1 = down;
			break;
		case winrt::Windows::System::VirtualKey::F2:
			keystate->F2 = down;
			break;
		case winrt::Windows::System::VirtualKey::F3:
			keystate->F3 = down;
			break;
		case winrt::Windows::System::VirtualKey::F4:
			keystate->F4 = down;
			break;
		case winrt::Windows::System::VirtualKey::F5:
			keystate->F5 = down;
			break;
		case winrt::Windows::System::VirtualKey::F6:
			keystate->F6 = down;
			break;
		case winrt::Windows::System::VirtualKey::F7:
			keystate->F7 = down;
			break;
		case winrt::Windows::System::VirtualKey::F8:
			keystate->F8 = down;
			break;
		case winrt::Windows::System::VirtualKey::F9:
			keystate->F9 = down;
			break;
		case winrt::Windows::System::VirtualKey::F10:
			keystate->F10 = down;
			break;
		case winrt::Windows::System::VirtualKey::F11:
			keystate->F11 = down;
			break;
		case winrt::Windows::System::VirtualKey::F12:
			keystate->F12 = down;
			break;
		case winrt::Windows::System::VirtualKey::F13:
			keystate->F13 = down;
			break;
		case winrt::Windows::System::VirtualKey::F14:
			keystate->F14 = down;
			break;
		case winrt::Windows::System::VirtualKey::F15:
			keystate->F15 = down;
			break;
		case winrt::Windows::System::VirtualKey::F16:
			keystate->F16 = down;
			break;
		case winrt::Windows::System::VirtualKey::F17:
			keystate->F17 = down;
			break;
		case winrt::Windows::System::VirtualKey::F18:
			keystate->F18 = down;
			break;
		case winrt::Windows::System::VirtualKey::F19:
			keystate->F19 = down;
			break;
		case winrt::Windows::System::VirtualKey::F20:
			keystate->F20 = down;
			break;
		case winrt::Windows::System::VirtualKey::F21:
			keystate->F21 = down;
			break;
		case winrt::Windows::System::VirtualKey::F22:
			keystate->F22 = down;
			break;
		case winrt::Windows::System::VirtualKey::F23:
			keystate->F23 = down;
			break;
		case winrt::Windows::System::VirtualKey::F24:
			keystate->F24 = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationView:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationMenu:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationUp:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationDown:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationLeft:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationRight:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationAccept:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NavigationCancel:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::NumberKeyLock:
			keystate->NumLock = down;
			break;
		case winrt::Windows::System::VirtualKey::Scroll:
			keystate->Scroll = down;
			break;
		case winrt::Windows::System::VirtualKey::LeftShift:
			keystate->LeftShift = down;
			break;
		case winrt::Windows::System::VirtualKey::RightShift:
			keystate->RightShift = down;
			break;
		case winrt::Windows::System::VirtualKey::LeftControl:
			keystate->LeftControl = down;
			break;
		case winrt::Windows::System::VirtualKey::RightControl:
			keystate->RightControl = down;
			break;
		case winrt::Windows::System::VirtualKey::LeftMenu:
			keystate->LeftAlt = down;
			break;
		case winrt::Windows::System::VirtualKey::RightMenu:
			keystate->RightAlt = down;
			break;
		case winrt::Windows::System::VirtualKey::GoBack:
			keystate->BrowserBack = down;
			break;
		case winrt::Windows::System::VirtualKey::GoForward:
			keystate->BrowserForward = down;
			break;
		case winrt::Windows::System::VirtualKey::Refresh:
			keystate->BrowserRefresh = down;
			break;
		case winrt::Windows::System::VirtualKey::Stop:
			keystate->BrowserStop = down;
			break;
		case winrt::Windows::System::VirtualKey::Search:
			keystate->BrowserSearch = down;
			break;
		case winrt::Windows::System::VirtualKey::Favorites:
			keystate->BrowserFavorites = down;
			break;
		case winrt::Windows::System::VirtualKey::GoHome:
			keystate->BrowserHome = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadA:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadB:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadX:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadY:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightShoulder:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftShoulder:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftTrigger:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightTrigger:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadUp:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadDown:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadLeft:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadRight:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadMenu:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadView:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickButton:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickButton:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickUp:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickDown:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickRight:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickLeft:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickUp:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickDown:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickRight:
			//keystate-> = down;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickLeft:
			//keystate-> = down;
			break;
		default:
			break;
		}
	}

	void InputManager::OnKeyDown(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::KeyEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::System::VirtualKey Key;
		Key = Args.VirtualKey();

		HandleKeys(&m_keyboardState, Key, true);
	}
	void InputManager::OnKeyUp(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::KeyEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::System::VirtualKey Key;
		Key = Args.VirtualKey();

		HandleKeys(&m_keyboardState, Key, false);
	}

	void InputManager::OnGamepadAdded(_In_ winrt::Windows::Foundation::IInspectable const& Sender, _In_ winrt::Windows::Gaming::Input::Gamepad const& Gamepad)
	{
		UNREFERENCED_PARAMETER(Sender);
		UNREFERENCED_PARAMETER(Gamepad);

		ScanGamePad();
	}
	void InputManager::OnGamepadRemoved(_In_ winrt::Windows::Foundation::IInspectable const& Sender, _In_  winrt::Windows::Gaming::Input::Gamepad const& Gamepad)
	{
		UNREFERENCED_PARAMETER(Sender);
		UNREFERENCED_PARAMETER(Gamepad);

		ScanGamePad();
	}

	void InputManager::OnPointerPressed(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_  winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		UpdatePointerDevices(pointer);
	}
	void InputManager::OnPointerMoved(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		if (pointer.IsInContact())
		{
			UpdatePointerDevices(pointer);
		}
	}
	void InputManager::OnPointerReleased(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		RemovePointerDevice(pointer);
	}
	void InputManager::OnPointerEntered(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		if (pointer.IsInContact())
		{
			UpdatePointerDevices(pointer);
		}
	}
	void InputManager::OnPointerExited(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		RemovePointerDevice(pointer);
	}
	void InputManager::OnPointerWheelChanged(_In_ winrt::Windows::UI::Core::CoreWindow const& Sender, _In_ winrt::Windows::UI::Core::PointerEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(Sender);

		winrt::Windows::UI::Input::PointerPoint pointer = Args.CurrentPoint();

		if (pointer.IsInContact())
		{
			UpdatePointerDevices(pointer);
		}
	}

	void InputManager::OnMouseMoved(_In_ winrt::Windows::Devices::Input::MouseDevice const& MouseDevice, _In_ winrt::Windows::Devices::Input::MouseEventArgs const& Args)
	{
		UNREFERENCED_PARAMETER(MouseDevice);

		m_mouseDelta.X = Args.MouseDelta().X;
		m_mouseDelta.Y = Args.MouseDelta().Y;
	}
	void InputManager::UpdatePointerDevices(_In_ winrt::Windows::UI::Input::PointerPoint const& device)
	{
		auto nr = 0U;

		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerId() == device.PointerId())
			{
				m_pointerdevices->at(nr) = device;
			}
			nr++;
		}
	}
	void InputManager::RemovePointerDevice(_In_ winrt::Windows::UI::Input::PointerPoint const& device)
	{
		auto nr = 0U;

		for (winrt::com_array<winrt::Windows::UI::Input::PointerPoint>::iterator it = m_pointerdevices->begin(); it != m_pointerdevices->end(); ++it)
		{
			if (it->PointerId() == device.PointerId())
			{
				m_pointerdevices->at(nr) = nullptr;
			}
			nr++;
		}
	}
} // namespace WOtech