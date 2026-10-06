////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: Window.cpp
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		07.08.2021
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Window.hpp"

namespace WOtech
{
	Window::Window() : m_corewindow{ nullptr }
	{
	}

	Window::Window(Window_Properties const& properties) : m_corewindow{ nullptr }
	{
		UNREFERENCED_PARAMETER(properties);
	}

	Window::~Window()
	{
		close();
	}

	void Window::close()
	{
		if (m_corewindow)
		{
			m_corewindow.Close();
			m_corewindow = nullptr;
		}
	}

	void Window::capturePointer()
	{
		if (m_corewindow)
		{
			m_corewindow.SetPointerCapture();
		}
	}

	void Window::releasePointer()
	{
		if (m_corewindow)
		{
			m_corewindow.ReleasePointerCapture();
		}
	}

	// Getters
	HWND Window::getWindow() const
	{
		auto windowNative{ m_corewindow.as<::IWindowNative>()};
		HWND hWnd{ 0 };
		windowNative->get_WindowHandle(&hWnd);
		return hWnd;
	}

	winrt::Windows::UI::Core::CoreWindow Window::getCoreWindow() const
	{
		return m_corewindow;
	}

	bool Window::isVisible() const
	{
		if (m_corewindow)
		{
			return m_corewindow.Visible();
		}
		return false;
	}


	bool Window::isValid() const
	{
		return static_cast<bool>(m_corewindow);
	}

	winrt::Windows::Foundation::Rect Window::getBounds() const
	{
		if (m_corewindow)
		{
			return m_corewindow.Bounds();
		}

		return winrt::Windows::Foundation::Rect();
	}

	
}
