////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: Utilities.h
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		07.08.2021
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_UTILITIES_H
#define WO_UTILITIES_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"

/////////////
// Defines //
/////////////
constexpr auto WO_PI = 3.141592241f;

namespace WOtech
{
	// Releases a COM object and nullifies pointer.
	template <typename InterfaceType>
	inline void SafeRelease(InterfaceType** currentObject)
	{
		if (*currentObject != NULL)
		{
			(*currentObject)->Release();
			*currentObject = NULL;
		}
	}

	// Acquires an additional reference, if non-null.
	template <typename InterfaceType>
	inline InterfaceType* SafeAcquire(InterfaceType* newObject)
	{
		if (newObject != NULL)
			newObject->AddRef();

		return newObject;
	}

	// Sets a new COM object, releasing the old one.
	template <typename InterfaceType>
	inline void SafeSet(InterfaceType** currentObject, InterfaceType* newObject)
	{
		SafeAcquire(newObject);
		SafeRelease(&currentObject);
		currentObject = newObject;
	}

	// Maps exceptions to equivalent HRESULTs,
	inline HRESULT ExceptionToHResult() throw()
	{
		try
		{
			throw;  // Rethrow previous exception.
		}
		catch (std::bad_alloc&)
		{
			return E_OUTOFMEMORY;
		}
		catch (...)
		{
			return E_FAIL;
		}
	}

	// Convert DirectX error codes to exceptions.
	inline void ThrowIfFailed(_In_ HRESULT const& hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DX API errors.
			throw std::system_error(hr, std::system_category());
		}
	}

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	inline FLOAT ConvertDipsToPixels(_In_ FLOAT const& dips, _In_ FLOAT const& dpi)
	{
		static const FLOAT dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

	// Converts Radian to Degree
	inline FLOAT RadiantoDegree(_In_ FLOAT const& radian)
	{
		FLOAT degree;

		degree = radian * (100 / WO_PI);

		return degree;
	}
	// Converts Degree to Radians
	inline FLOAT DegreetoRadian(_In_ FLOAT const& degree)
	{
		FLOAT radian;

		radian = degree * (WO_PI / 180);

		return radian;
	}
	// Converts Vector to Degree
	inline FLOAT VectortoRadian(_In_ FLOAT const& x, _In_ FLOAT const& y)
	{
		FLOAT result = atan2(y, x);

		return result;
	}
	// Converts Vector to Degree
	inline FLOAT VectortoDegree(_In_ FLOAT const& x, _In_ FLOAT const& y)
	{
		FLOAT result = RadiantoDegree(VectortoRadian(y, x));

		return result;
	}

	// Ceck if value is between min and max
	template <typename T>
	inline T InRange(_In_ T value, _In_ T min, _In_ T max)
	{
		T output;

		if (value < min)
			output = min;
		else if (value > max)
			output = max;
		else
			output = value;

		return output;
	}

	// Check for SDK D3D11 Layer support.
	inline bool SDK11LayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
		);

		return SUCCEEDED(hr);
	}

	//  Template Singleton Class
	template <class T>
	class TSingleton
	{
	protected:

		static T* m_pSingleton;

	public:

		virtual ~TSingleton()
		{
		} // Destructor

		inline static T* Get()
		{
			if (!m_pSingleton)
				m_pSingleton = new T;

			return(m_pSingleton);
		}// Get

		static void Del()
		{
			if (m_pSingleton)
			{
				delete (m_pSingleton);
				m_pSingleton = NULL;
			}
		}//Del
	};

	template <class T>
	T* TSingleton<T>::m_pSingleton = 0;

	// Convert a wide Unicode string to an UTF8 string
	inline std::string utf8_encode(_In_ winrt::hstring const& wstr)
	{
		if (wstr.empty()) return std::string();
		INT size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (INT)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (INT)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	inline winrt::hstring utf8_decode(_In_ std::string const& str)
	{
		if (str.empty()) return winrt::hstring();
		//INT size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (INT)str.size(), NULL, 0);
		winrt::hstring wstrTo = winrt::to_hstring(str.data());
		//MultiByteToWideChar(CP_UTF8, 0, &str[0], (INT)str.size(), &wstrTo[0], size_needed); todo: hmm
		return wstrTo;
	}
}//namespace WOtech
#endif