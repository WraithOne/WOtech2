////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Audio.h
///
///			Description:
///
///			Created:	20.09.2014
///			Edited:		23.09.2018
///
////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Audio.hpp"
#include "MediaReader.hpp"

namespace WOtech
{
	AudioEngine::AudioEngine()
	{
		m_audioAvailable = false;
		m_effectMasterVoice = nullptr;
		m_musicMasterVoice = nullptr;
	}

	AudioEngine::~AudioEngine()
	{
		if (m_effectMasterVoice != nullptr)
		{
			m_effectMasterVoice->DestroyVoice();
			m_effectDevice = nullptr;
		}

		if (m_musicMasterVoice != nullptr)
		{
			m_musicMasterVoice->DestroyVoice();
			m_musicDevice = nullptr;
		}
	}

	void AudioEngine::Initialize()
	{
		//default Processor
		WOtech::AUDIO_PROCESSOR xaProcessor = WOtech::AUDIO_PROCESSOR::AUDIO_PROCESSOR_DEFAULT_PROCESSOR;
		CreateDeviceIndependentResources(xaProcessor);

		// Default Device
		UINT devCount = 0;

		GetDeviceCount(m_effectDevice.Get(), &devCount);

		auto devDetails = winrt::com_array<WOtech::DeviceDetails>(devCount);
		UINT i = 0;
		for (i = 0; i < devDetails.size(); i++)
		{
			GetDeviceDetails(m_effectDevice.Get(), i, &devDetails[i]);
			if (devDetails[i].isDefault)
				break;
		}

		CreateDevicedependentResources(devDetails[i].DeviceID);
	}

	void AudioEngine::Initialize(_In_ WOtech::AUDIO_PROCESSOR const& xaProcessor, _In_ winrt::hstring  const& deviceID)
	{
		CreateDeviceIndependentResources(xaProcessor);
		CreateDevicedependentResources(deviceID);
	}

	void AudioEngine::CreateDeviceIndependentResources(_In_ WOtech::AUDIO_PROCESSOR  const& xaProcessor)
	{
		HRESULT hr;
		UINT Flags = NULL;

		// Create the effect engine
		hr = XAudio2Create(&m_effectDevice, Flags, xaProcessor);
		ThrowIfFailed(hr);

		// Create the music engine
		hr = XAudio2Create(&m_musicDevice, Flags, xaProcessor);
		ThrowIfFailed(hr);

#if defined(_DEBUG)
		// Set the device�s to debug
		XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
		debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
		debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
		m_effectDevice->SetDebugConfiguration(&debugConfiguration);
		m_musicDevice->SetDebugConfiguration(&debugConfiguration);
#endif
	}

	void AudioEngine::CreateDevicedependentResources(_In_ winrt::hstring  const& deviceID)
	{
		HRESULT hr;
		UINT flags = 0;

		// Check if devices are valid
		if (m_effectDevice == nullptr || m_musicDevice == nullptr)
			ThrowIfFailed(XAUDIO2_E_DEVICE_INVALIDATED);

		// Create Effectvoice
		hr = m_effectDevice->CreateMasteringVoice(&m_effectMasterVoice,
			XAUDIO2_DEFAULT_CHANNELS,
			XAUDIO2_DEFAULT_SAMPLERATE,
			flags,
			deviceID.data(),
			NULL,
			_AUDIO_STREAM_CATEGORY::AudioCategory_GameEffects);
		ThrowIfFailed(hr);

		// Create Musicvoice
		hr = m_musicDevice->CreateMasteringVoice(&m_musicMasterVoice,
			XAUDIO2_DEFAULT_CHANNELS,
			XAUDIO2_DEFAULT_SAMPLERATE,
			flags,
			deviceID.data(),
			NULL,
			_AUDIO_STREAM_CATEGORY::AudioCategory_GameMedia);
		ThrowIfFailed(hr);

		m_audioAvailable = true;
	}

	void AudioEngine::SuspendAudio()
	{
		if (m_audioAvailable)
		{
			m_effectDevice->StopEngine();
			m_musicDevice->StopEngine();
		}
	}

	void AudioEngine::ResumeAudio()
	{
		HRESULT hr;

		if (m_audioAvailable)
		{
			// resume Effect engine
			hr = m_effectDevice->StartEngine();
			ThrowIfFailed(hr);
			// Resume music engine
			hr = m_musicDevice->StartEngine();
			ThrowIfFailed(hr);
		}
		else
		{
			ThrowIfFailed(XAUDIO2_E_INVALID_CALL);
		}
	}

	void AudioEngine::SetMasterVolume(_In_ FLOAT  const& effectVolume, _In_ FLOAT  const& musicVolume)
	{
		HRESULT hr = XAUDIO2_E_INVALID_CALL;

		if (m_effectMasterVoice)
		{
			hr = m_effectMasterVoice->SetVolume(effectVolume, NULL);
			ThrowIfFailed(hr);
		}

		if (m_musicMasterVoice)
		{
			hr = m_musicMasterVoice->SetVolume(musicVolume, NULL);
			ThrowIfFailed(hr);
		}

		ThrowIfFailed(hr);
	}

	IXAudio2* AudioEngine::GetEffectEngine()
	{
		return m_effectDevice.Get();
	}

	IXAudio2* AudioEngine::GetMusicEngine()
	{
		return m_musicDevice.Get();
	}

	void AudioEngine::GetMasterVolome(_Out_ FLOAT* effectVolume, _Out_ FLOAT* musicVolume)
	{
		FLOAT failed = 0.0f;

		if (m_effectMasterVoice)
			m_effectMasterVoice->GetVolume(effectVolume);
		else
			effectVolume = &failed;

		if (m_musicMasterVoice)
			m_musicMasterVoice->GetVolume(musicVolume);
		else
			musicVolume = &failed;
	}

	void AudioEngine::GetDeviceCount(_In_ IXAudio2* const& device, _Out_ UINT* devCount)
	{
		UNREFERENCED_PARAMETER(device);

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
		// There doesn't seem to be any audio device enumeration on Windows Phone.
		// In lieu of this, just treat things as if there is one and only one
		// audio device.
		* devcount = 1;
#else
		// TODO, WinRT: make xaudio2 device enumeration only happen once, and in the background

		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Devices::Enumeration::DeviceInformationCollection> operation = winrt::Windows::Devices::Enumeration::DeviceInformation::FindAllAsync(winrt::Windows::Devices::Enumeration::DeviceClass::AudioRender);
		while (operation.Status() != winrt::Windows::Foundation::AsyncStatus::Completed);

		winrt::Windows::Devices::Enumeration::DeviceInformationCollection devices = operation.GetResults();
		UINT size = 0U;
		for (const winrt::Windows::Devices::Enumeration::DeviceInformation& deviceInfo : devices)
		{
			UNREFERENCED_PARAMETER(deviceInfo);
			size++;
		}

		*devCount = size;
#endif
	}

	void AudioEngine::GetDeviceDetails(_In_ IXAudio2* const& device, _In_  UINT  const& index, _Out_ DeviceDetails* details)
	{
		UNREFERENCED_PARAMETER(device);

		auto defaultDevice = winrt::Windows::Media::Devices::MediaDevice::GetDefaultAudioRenderId(winrt::Windows::Media::Devices::AudioDeviceRole::Default);

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
		// Windows Phone doesn't seem to have the same device enumeration APIs that
		// Windows 8/RT has, or it doesn't have them at all.  In lieu of this,
		// just treat things as if there is one, and only one, default device.
		if (index != 0)
		{
			return XAUDIO2_E_INVALID_CALL;
		}

		if (details)
		{
			wcsncpy_s(details->DeviceID, ARRAYSIZE(details->DeviceID), L"default", _TRUNCATE);
			wcsncpy_s(details->DisplayName, ARRAYSIZE(details->DisplayName), L"default", _TRUNCATE);
		}
		return S_OK;
#else
		auto operation = winrt::Windows::Devices::Enumeration::DeviceInformation::FindAllAsync(winrt::Windows::Devices::Enumeration::DeviceClass::AudioRender);
		while (operation.Status() != winrt::Windows::Foundation::AsyncStatus::Completed);

		auto devices = operation.GetResults();
		if (index >= devices.Size())
		{
			WOtech::ThrowIfFailed(XAUDIO2_E_INVALID_CALL);
		}

		auto d = devices.GetAt(index);

		details->DeviceID = d.Id();
		details->DisplayName = d.Name();
		details->isDefault = d.Id() == defaultDevice;
		details->isEnabled = d.IsEnabled();
#endif
	}
}//namespace WOtech