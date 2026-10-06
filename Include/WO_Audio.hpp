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
///			Header file for AudioEngine and AudioSource
///
///			Created:	01.05.2014
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_AUDIO_H
#define WO_AUDIO_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "..\Include\WO_Utilities.hpp"
#include "..\Include\WO_AudioComponents.hpp"

namespace WOtech
{
	class AudioEngine
	{
	public:
		AudioEngine();

		void Initialize();
		void Initialize(_In_ WOtech::AUDIO_PROCESSOR const& xaProcessor, _In_ winrt::hstring const& deviceID);

		void CreateDeviceIndependentResources(_In_ WOtech::AUDIO_PROCESSOR const& xaProcessor);
		void CreateDevicedependentResources(_In_ winrt::hstring const& deviceID);

		void SuspendAudio();
		void ResumeAudio();

		void SetMasterVolume(_In_ FLOAT const& effectVolume, _In_ FLOAT const& musicVolume);
		void GetMasterVolome(_Out_ FLOAT* effectVolume, _Out_ FLOAT* musicVolume);

		IXAudio2* GetEffectEngine();
		IXAudio2* GetMusicEngine();

		void GetDeviceCount(_In_ IXAudio2* const& device, _Out_ UINT* devCount);
		void GetDeviceDetails(_In_ IXAudio2* const& device, _In_  UINT const& index, _Out_ WOtech::DeviceDetails* details);

	private:
		~AudioEngine();

	private:
		bool								m_audioAvailable;

		Microsoft::WRL::ComPtr<IXAudio2>	m_effectDevice;
		IXAudio2MasteringVoice*				m_effectMasterVoice;

		Microsoft::WRL::ComPtr<IXAudio2>	m_musicDevice;
		IXAudio2MasteringVoice*				m_musicMasterVoice;
	};

	class AudioSource
	{
	public:
		AudioSource(_In_ winrt::hstring const& fileName, _In_ WOtech::AudioEngine* const& audioEngine, _In_ WOtech::AUDIO_TYPE const& audioType);

		void LoadWave();

		void Play();
		void Pause();
		void Resume();
		void Stop();

		void setVolume(_In_ FLOAT const& volume);
		void getVolume(_Out_ FLOAT* volume);

		void getPlaybackState(_Out_ WOtech::AUDIO_PLAYBACK_STATE* playbackState);
		void getState(_Out_ WOtech::AudioSourceState* state);

	private:
		~AudioSource();

		void CreateSourceVoice();

		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			std::atomic<bool> finished;
			VoiceCallback() : finished(false) {}
			~VoiceCallback() {}

			//Called when the voice has just finished playing a contiguous audio stream.
			STDMETHOD_(void, OnStreamEnd)() { finished = true; }

			//Unused methods are stubs
			STDMETHOD_(void, OnVoiceProcessingPassEnd) () {}
			STDMETHOD_(void, OnVoiceProcessingPassStart) (_In_ UINT SamplesRequired) { UNREFERENCED_PARAMETER(SamplesRequired); }
			STDMETHOD_(void, OnBufferEnd) (_In_ void* pBufferContext) { UNREFERENCED_PARAMETER(pBufferContext); finished = true; }
			STDMETHOD_(void, OnBufferStart) (_In_ void* pBufferContext) { UNREFERENCED_PARAMETER(pBufferContext); finished = false; }
			STDMETHOD_(void, OnLoopEnd) (_In_ void* pBufferContext) { UNREFERENCED_PARAMETER(pBufferContext); }
			STDMETHOD_(void, OnVoiceError) (_In_ void* pBufferContext, _In_ HRESULT Error) { UNREFERENCED_PARAMETER(pBufferContext); ThrowIfFailed(Error); }
		};

	private:
		winrt::hstring					m_fileName;
		WOtech::AudioEngine*			m_audioEngine;
		WOtech::AUDIO_TYPE				m_audioType;

		bool							m_audioAvailable;
		WOtech::AUDIO_PLAYBACK_STATE	m_playbackState;

		VoiceCallback					m_voiceCallback;
		WAVEFORMATEX*					m_sourceFormat;
		IXAudio2SourceVoice*			m_sourceVoice;
		winrt::com_array<byte>			m_soundData;
	};
}
#endif