//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#ifndef MEDIAREADER_H
#define MEDIAREADER_H

// MediaReader:
// This is a helper class for the SoundEffect class.  It reads small audio files
// synchronously from the package installed folder and returns sound data as a
// byte array.


#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfuuid.lib")

class MediaReader
{
public:
	MediaReader();

	winrt::com_array<byte>	LoadMedia(_In_ winrt::hstring const& filename);
	WAVEFORMATEX* GetOutputWaveFormatEx();

private:
	winrt::hstring	m_installedLocationPath;
	WAVEFORMATEX	m_waveFormat;
};
#endif // !MEDIAREADER_H
