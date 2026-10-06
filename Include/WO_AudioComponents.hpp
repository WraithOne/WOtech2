////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: AudioComponents.h
///
///			Description:
///			Header file for Audio Components
///
///			Created:	06.10.2017
///			Edited:		23.09.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_AUDIOCOMPONENTS_H
#define WO_AUDIOCOMPONENTS_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

/////////////
// LINKING //
/////////////

namespace WOtech
{
	enum AUDIO_PROCESSOR
	{
		AUDIO_PROCESSOR_PROCESSOR1,
		AUDIO_PROCESSOR_PROCESSOR2,
		AUDIO_PROCESSOR_PROCESSOR3,
		AUDIO_PROCESSOR_PROCESSOR4,
		AUDIO_PROCESSOR_PROCESSOR5,
		AUDIO_PROCESSOR_PROCESSOR6,
		AUDIO_PROCESSOR_PROCESSOR7,
		AUDIO_PROCESSOR_PROCESSOR8,
		AUDIO_PROCESSOR_PROCESSOR9,
		AUDIO_PROCESSOR_PROCESSOR10,
		AUDIO_PROCESSOR_PROCESSOR11,
		AUDIO_PROCESSOR_PROCESSOR12,
		AUDIO_PROCESSOR_PROCESSOR13,
		AUDIO_PROCESSOR_PROCESSOR14,
		AUDIO_PROCESSOR_PROCESSOR15,
		AUDIO_PROCESSOR_PROCESSOR16,
		AUDIO_PROCESSOR_PROCESSOR17,
		AUDIO_PROCESSOR_PROCESSOR18,
		AUDIO_PROCESSOR_PROCESSOR19,
		AUDIO_PROCESSOR_PROCESSOR20,
		AUDIO_PROCESSOR_PROCESSOR21,
		AUDIO_PROCESSOR_PROCESSOR22,
		AUDIO_PROCESSOR_PROCESSOR23,
		AUDIO_PROCESSOR_PROCESSOR24,
		AUDIO_PROCESSOR_PROCESSOR25,
		AUDIO_PROCESSOR_PROCESSOR26,
		AUDIO_PROCESSOR_PROCESSOR27,
		AUDIO_PROCESSOR_PROCESSOR28,
		AUDIO_PROCESSOR_PROCESSOR29,
		AUDIO_PROCESSOR_PROCESSOR30,
		AUDIO_PROCESSOR_PROCESSOR31,
		AUDIO_PROCESSOR_PROCESSOR32,
		AUDIO_PROCESSOR_ANY_PROCESSOR,
		AUDIO_PROCESSOR_DEFAULT_PROCESSOR,
	};

	enum AUDIO_TYPE
	{
		AUDIO_TYPE_EFFECT,
		AUDIO_TYPE_MUSIC
	};

	enum AUDIO_PLAYBACK_STATE
	{
		AUDIO_PLAYBACK_STATE_STOPPED,
		AUDIO_PLAYBACK_STATE_PLAYING,
		AUDIO_PLAYBACK_STATE_PAUSED
	};

	class BufferContext
	{
	public:
		BufferContext();

		void setBufferContext(_In_ void* const& pCurrentBufferContext);

		void getBufferContext(_Out_ void* pCurrentBufferContext);
	private:
		void* m_pCurrentBufferContext = nullptr;
	};

	class VoiceState
	{
	public:
		VoiceState()
		{
			BuffersQueued = 0;
			SamplesPlayed = 0;
			pCurrentBufferContext = new BufferContext;
		}
		UINT32 BuffersQueued;
		UINT64 SamplesPlayed;

		BufferContext* pCurrentBufferContext;
	};

	class AudioSourceState
	{
	public:
		AudioSourceState()
		{
			PlaybackState = AUDIO_PLAYBACK_STATE_STOPPED;
			VoiceState = new WOtech::VoiceState;
		}
		AUDIO_PLAYBACK_STATE PlaybackState;

		VoiceState* VoiceState;
	};

	struct DeviceDetails
	{
		winrt::hstring DeviceID;
		winrt::hstring DisplayName;
		bool isDefault;
		bool isEnabled;
	};
}
#endif