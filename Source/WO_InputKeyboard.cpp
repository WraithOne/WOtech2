////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputKeyboard.cpp
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
	bool InputManager::KeyboardConnected()
	{
		if (m_keyBoardCapabilities.KeyboardPresent() >= 1)
			return true;

		return false;
	}// InputManager KeyboardConnected

	Keyboard_State InputManager::getKeyboardState()
	{
		return m_keyboardState;
	}

	bool InputManager::KeyDown(_In_ winrt::Windows::System::VirtualKey const& Key)
	{
		// TODO: not handcoded
		switch (Key)
		{
		case winrt::Windows::System::VirtualKey::None:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::LeftButton:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::RightButton:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::Cancel:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::MiddleButton:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::XButton1:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::XButton2:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::Back:
			return m_keyboardState.Back;
			break;
		case winrt::Windows::System::VirtualKey::Tab:
			return m_keyboardState.Tab;
			break;
		case winrt::Windows::System::VirtualKey::Clear:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::Enter:
			return m_keyboardState.Enter;
			break;
		case winrt::Windows::System::VirtualKey::Shift:
			return m_keyboardState.LeftShift || m_keyboardState.RightShift;
			break;
		case winrt::Windows::System::VirtualKey::Control:
			return m_keyboardState.LeftControl || m_keyboardState.RightControl;
			break;
		case winrt::Windows::System::VirtualKey::Menu:
			return m_keyboardState.LeftAlt || m_keyboardState.RightAlt;
			break;
		case winrt::Windows::System::VirtualKey::Pause:
			return m_keyboardState.Pause;
			break;
		case winrt::Windows::System::VirtualKey::CapitalLock:
			return m_keyboardState.CapsLock;
			break;
		case winrt::Windows::System::VirtualKey::Kana:
			return m_keyboardState.Kana;
			break;
			//case winrt::Windows::System::VirtualKey::Hangul:
				//return false;
				//break;
		case winrt::Windows::System::VirtualKey::Junja:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::Final:
			return false;
			break;
			//case winrt::Windows::System::VirtualKey::Hanja:
				//return m_keyboardState.;
				//break;
		case winrt::Windows::System::VirtualKey::Kanji:
			return m_keyboardState.Kanji;
			break;
		case winrt::Windows::System::VirtualKey::Escape:
			return m_keyboardState.Escape;
			break;
		case winrt::Windows::System::VirtualKey::Convert:
			return m_keyboardState.ImeConvert;
			break;
		case winrt::Windows::System::VirtualKey::NonConvert:
			return m_keyboardState.ImeNoConvert;
			break;
		case winrt::Windows::System::VirtualKey::Accept:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::ModeChange:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::Space:
			return m_keyboardState.Space;
			break;
		case winrt::Windows::System::VirtualKey::PageUp:
			return m_keyboardState.PageUp;
			break;
		case winrt::Windows::System::VirtualKey::PageDown:
			return m_keyboardState.PageDown;
			break;
		case winrt::Windows::System::VirtualKey::End:
			return m_keyboardState.End;
			break;
		case winrt::Windows::System::VirtualKey::Home:
			return m_keyboardState.Home;
			break;
		case winrt::Windows::System::VirtualKey::Left:
			return m_keyboardState.Left;
			break;
		case winrt::Windows::System::VirtualKey::Up:
			return m_keyboardState.Up;
			break;
		case winrt::Windows::System::VirtualKey::Right:
			return m_keyboardState.Right;
			break;
		case winrt::Windows::System::VirtualKey::Down:
			return m_keyboardState.Down;
			break;
		case winrt::Windows::System::VirtualKey::Select:
			return m_keyboardState.Select;
			break;
		case winrt::Windows::System::VirtualKey::Print:
			return m_keyboardState.Print;
			break;
		case winrt::Windows::System::VirtualKey::Execute:
			return m_keyboardState.Execute;
			break;
		case winrt::Windows::System::VirtualKey::Snapshot:
			return m_keyboardState.PrintScreen;
			break;
		case winrt::Windows::System::VirtualKey::Insert:
			return m_keyboardState.Insert;
			break;
		case winrt::Windows::System::VirtualKey::Delete:
			return m_keyboardState.Delete;
			break;
		case winrt::Windows::System::VirtualKey::Help:
			return m_keyboardState.Help;
			break;
		case winrt::Windows::System::VirtualKey::Number0:
			return m_keyboardState.D0;
			break;
		case winrt::Windows::System::VirtualKey::Number1:
			return m_keyboardState.D1;
			break;
		case winrt::Windows::System::VirtualKey::Number2:
			return m_keyboardState.D2;
			break;
		case winrt::Windows::System::VirtualKey::Number3:
			return m_keyboardState.D3;
			break;
		case winrt::Windows::System::VirtualKey::Number4:
			return m_keyboardState.D4;
			break;
		case winrt::Windows::System::VirtualKey::Number5:
			return m_keyboardState.D5;
			break;
		case winrt::Windows::System::VirtualKey::Number6:
			return m_keyboardState.D6;
			break;
		case winrt::Windows::System::VirtualKey::Number7:
			return m_keyboardState.D7;
			break;
		case winrt::Windows::System::VirtualKey::Number8:
			return m_keyboardState.D8;
			break;
		case winrt::Windows::System::VirtualKey::Number9:
			return m_keyboardState.D9;
			break;
		case winrt::Windows::System::VirtualKey::A:
			return m_keyboardState.A;
			break;
		case winrt::Windows::System::VirtualKey::B:
			return m_keyboardState.B;
			break;
		case winrt::Windows::System::VirtualKey::C:
			return m_keyboardState.C;
			break;
		case winrt::Windows::System::VirtualKey::D:
			return m_keyboardState.D;
			break;
		case winrt::Windows::System::VirtualKey::E:
			return m_keyboardState.E;
			break;
		case winrt::Windows::System::VirtualKey::F:
			return m_keyboardState.F;
			break;
		case winrt::Windows::System::VirtualKey::G:
			return m_keyboardState.G;
			break;
		case winrt::Windows::System::VirtualKey::H:
			return m_keyboardState.H;
			break;
		case winrt::Windows::System::VirtualKey::I:
			return m_keyboardState.I;
			break;
		case winrt::Windows::System::VirtualKey::J:
			return m_keyboardState.J;
			break;
		case winrt::Windows::System::VirtualKey::K:
			return m_keyboardState.K;
			break;
		case winrt::Windows::System::VirtualKey::L:
			return m_keyboardState.L;
			break;
		case winrt::Windows::System::VirtualKey::M:
			return m_keyboardState.M;
			break;
		case winrt::Windows::System::VirtualKey::N:
			return m_keyboardState.N;
			break;
		case winrt::Windows::System::VirtualKey::O:
			return m_keyboardState.O;
			break;
		case winrt::Windows::System::VirtualKey::P:
			return m_keyboardState.P;
			break;
		case winrt::Windows::System::VirtualKey::Q:
			return m_keyboardState.Q;
			break;
		case winrt::Windows::System::VirtualKey::R:
			return m_keyboardState.R;
			break;
		case winrt::Windows::System::VirtualKey::S:
			return m_keyboardState.S;
			break;
		case winrt::Windows::System::VirtualKey::T:
			return m_keyboardState.T;
			break;
		case winrt::Windows::System::VirtualKey::U:
			return m_keyboardState.U;
			break;
		case winrt::Windows::System::VirtualKey::V:
			return m_keyboardState.V;
			break;
		case winrt::Windows::System::VirtualKey::W:
			return m_keyboardState.W;
			break;
		case winrt::Windows::System::VirtualKey::X:
			return m_keyboardState.X;
			break;
		case winrt::Windows::System::VirtualKey::Y:
			return m_keyboardState.Y;
			break;
		case winrt::Windows::System::VirtualKey::Z:
			return m_keyboardState.Z;
			break;
		case winrt::Windows::System::VirtualKey::LeftWindows:
			return m_keyboardState.LeftWindows;
			break;
		case winrt::Windows::System::VirtualKey::RightWindows:
			return m_keyboardState.RightWindows;
			break;
		case winrt::Windows::System::VirtualKey::Application:
			return m_keyboardState.Apps;
			break;
		case winrt::Windows::System::VirtualKey::Sleep:
			return m_keyboardState.Sleep;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad0:
			return m_keyboardState.NumPad0;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad1:
			return m_keyboardState.NumPad1;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad2:
			return m_keyboardState.NumPad2;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad3:
			return m_keyboardState.NumPad3;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad4:
			return m_keyboardState.NumPad4;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad5:
			return m_keyboardState.NumPad5;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad6:
			return m_keyboardState.NumPad6;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad7:
			return m_keyboardState.NumPad7;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad8:
			return m_keyboardState.NumPad8;
			break;
		case winrt::Windows::System::VirtualKey::NumberPad9:
			return m_keyboardState.NumPad9;
			break;
		case winrt::Windows::System::VirtualKey::Multiply:
			return m_keyboardState.Multiply;
			break;
		case winrt::Windows::System::VirtualKey::Add:
			return m_keyboardState.Add;
			break;
		case winrt::Windows::System::VirtualKey::Separator:
			return m_keyboardState.Separator;
			break;
		case winrt::Windows::System::VirtualKey::Subtract:
			return m_keyboardState.Subtract;
			break;
		case winrt::Windows::System::VirtualKey::Decimal:
			return m_keyboardState.Decimal;
			break;
		case winrt::Windows::System::VirtualKey::Divide:
			return m_keyboardState.Divide;
			break;
		case winrt::Windows::System::VirtualKey::F1:
			return m_keyboardState.F1;
			break;
		case winrt::Windows::System::VirtualKey::F2:
			return m_keyboardState.F2;
			break;
		case winrt::Windows::System::VirtualKey::F3:
			return m_keyboardState.F3;
			break;
		case winrt::Windows::System::VirtualKey::F4:
			return m_keyboardState.F4;
			break;
		case winrt::Windows::System::VirtualKey::F5:
			return m_keyboardState.F5;
			break;
		case winrt::Windows::System::VirtualKey::F6:
			return m_keyboardState.F6;
			break;
		case winrt::Windows::System::VirtualKey::F7:
			return m_keyboardState.F7;
			break;
		case winrt::Windows::System::VirtualKey::F8:
			return m_keyboardState.F8;
			break;
		case winrt::Windows::System::VirtualKey::F9:
			return m_keyboardState.F9;
			break;
		case winrt::Windows::System::VirtualKey::F10:
			return m_keyboardState.F10;
			break;
		case winrt::Windows::System::VirtualKey::F11:
			return m_keyboardState.F11;
			break;
		case winrt::Windows::System::VirtualKey::F12:
			return m_keyboardState.F12;
			break;
		case winrt::Windows::System::VirtualKey::F13:
			return m_keyboardState.F13;
			break;
		case winrt::Windows::System::VirtualKey::F14:
			return m_keyboardState.F14;
			break;
		case winrt::Windows::System::VirtualKey::F15:
			return m_keyboardState.F15;
			break;
		case winrt::Windows::System::VirtualKey::F16:
			return m_keyboardState.F16;
			break;
		case winrt::Windows::System::VirtualKey::F17:
			return m_keyboardState.F17;
			break;
		case winrt::Windows::System::VirtualKey::F18:
			return m_keyboardState.F18;
			break;
		case winrt::Windows::System::VirtualKey::F19:
			return m_keyboardState.F19;
			break;
		case winrt::Windows::System::VirtualKey::F20:
			return m_keyboardState.F20;
			break;
		case winrt::Windows::System::VirtualKey::F21:
			return m_keyboardState.F21;
			break;
		case winrt::Windows::System::VirtualKey::F22:
			return m_keyboardState.F22;
			break;
		case winrt::Windows::System::VirtualKey::F23:
			return m_keyboardState.F23;
			break;
		case winrt::Windows::System::VirtualKey::F24:
			return m_keyboardState.F24;
			break;
		case winrt::Windows::System::VirtualKey::NavigationView:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationMenu:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationUp:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationDown:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationLeft:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationRight:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationAccept:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NavigationCancel:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::NumberKeyLock:
			return m_keyboardState.NumLock;
			break;
		case winrt::Windows::System::VirtualKey::Scroll:
			return m_keyboardState.Scroll;
			break;
		case winrt::Windows::System::VirtualKey::LeftShift:
			return m_keyboardState.LeftShift;
			break;
		case winrt::Windows::System::VirtualKey::RightShift:
			return m_keyboardState.RightShift;
			break;
		case winrt::Windows::System::VirtualKey::LeftControl:
			return m_keyboardState.LeftControl;
			break;
		case winrt::Windows::System::VirtualKey::RightControl:
			return m_keyboardState.RightControl;
			break;
		case winrt::Windows::System::VirtualKey::LeftMenu:
			return m_keyboardState.LeftAlt;
			break;
		case winrt::Windows::System::VirtualKey::RightMenu:
			return m_keyboardState.RightAlt;
			break;
		case winrt::Windows::System::VirtualKey::GoBack:
			return m_keyboardState.BrowserBack;
			break;
		case winrt::Windows::System::VirtualKey::GoForward:
			return m_keyboardState.BrowserForward;
			break;
		case winrt::Windows::System::VirtualKey::Refresh:
			return m_keyboardState.BrowserRefresh;
			break;
		case winrt::Windows::System::VirtualKey::Stop:
			return m_keyboardState.BrowserStop;
			break;
		case winrt::Windows::System::VirtualKey::Search:
			return m_keyboardState.BrowserSearch;
			break;
		case winrt::Windows::System::VirtualKey::Favorites:
			return m_keyboardState.BrowserFavorites;
			break;
		case winrt::Windows::System::VirtualKey::GoHome:
			return m_keyboardState.BrowserHome;
			break;
		case winrt::Windows::System::VirtualKey::GamepadA:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadB:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadX:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadY:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightShoulder:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftShoulder:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftTrigger:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightTrigger:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadUp:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadDown:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadLeft:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadDPadRight:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadMenu:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadView:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickButton:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickButton:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickUp:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickDown:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickRight:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadLeftThumbstickLeft:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickUp:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickDown:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickRight:
			return false;
			break;
		case winrt::Windows::System::VirtualKey::GamepadRightThumbstickLeft:
			return false;
			break;
		default:
			return false;
			break;
		}
	}// InputManager KeyboardKeyDown
}//namespace WOtech