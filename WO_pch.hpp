////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine 2
///
///			https://github.com/WraithOne/WOtech22
///			by https://twitter.com/WraithOne
///
///			File: pch.h
///
///			Description:
///
///			Created:	01.08.2021
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#pragma once

//#pragma warning( disable : 4251 4264)
//#pragma warning(disable : 4324 4481)

// Windows SDK
#include "targetver.hpp"

/////////////
// Macros  //
/////////////

#define NOMINMAX
#define NOHELP
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include "WO_Macros.hpp"

// Winsock must precede Windows.h
#include <winsock2.h>
#include <ws2tcpip.h>

// Windows
#include <Windows.h>

// WRL
#include <wrl.h>
#include <wrl/client.h>
#include <concrt.h>

#include <ppltasks.h>
#include <stdio.h>
#include <wincodec.h>

#include <unknwn.h>

//Windows APP SDK
#include <microsoft.ui.xaml.window.h>

// C++/WinRT
#include <winrt/base.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.activation.h>
#include <winrt/Windows.Devices.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.Input.h>
#include <winrt/Windows.Devices.Input.Preview.h>
#include <winrt/Windows.Devices.Sensors.h>
#include <winrt/Windows.Devices.power.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Devices.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.Input.Core.h>
#include <winrt/Windows.Ui.Windowmanagement.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Provider.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.System.Threading.h>

// STL
#include <algorithm>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <math.h>

// DirectX
#include <DXGItype.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <d2d1_3helper.h>
#include <dwrite_3.h>

#include <d3dcompiler.h>

#include <DirectXColors.h>
#include <DirectXMath.h>

#include <xaudio2.h>
#include <xaudio2fx.h>

#include <Xinput.h>

/////////////
// LINKING //
/////////////
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "Dwrite.lib")
#pragma comment (lib, "Windowscodecs.lib")
#pragma comment (lib, "xaudio2.lib")
