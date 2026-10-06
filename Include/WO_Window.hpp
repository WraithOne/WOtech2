////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: Window.h
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		07.08.2021
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_WINDOW_H
#define WO_WINDOW_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"

namespace WOtech
{
	struct Window_Properties
	{
		std::string Name;
		RECT Bounds;
	};
	class Window
	{
	public:
		Window();
		Window(Window_Properties const& properties);

		~Window();

		void close();

		void capturePointer();
		void releasePointer();

		// Getters
		HWND getWindow() const;
		winrt::Windows::UI::Core::CoreWindow getCoreWindow() const;

		bool isVisible() const;
		bool isValid() const;

		winrt::Windows::Foundation::Rect getBounds() const;
		
	private:
		winrt::Windows::UI::Core::CoreWindow m_corewindow;
	};
} // namespace WOtech
#endif