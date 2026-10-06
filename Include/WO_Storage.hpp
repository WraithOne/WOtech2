////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Storage.h
///
///			Description:
///
///			Created:	09.01.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_STORAGE_H
#define WO_STORAGE_H

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"

namespace WOtech
{
	class RoamingDataHandler
	{
	public:
		RoamingDataHandler();

		void DataChangeHandler(_In_ winrt::Windows::Storage::ApplicationData const& appData, _In_ winrt::Windows::Foundation::IInspectable const& o);

		winrt::Windows::Storage::ApplicationDataContainer getRoamingSettings()
		{
			return m_roamingSettings;
		}
		winrt::Windows::Storage::StorageFolder getRoamingFolder()
		{
			return m_roamingFolder;
		}
		bool isRoamingChanged()
		{
			return m_roamingChanged;
		}
		void setRoamingChanged(bool changed)
		{
			m_roamingChanged = changed;
		}

	private:
		winrt::Windows::Storage::ApplicationDataContainer	m_roamingSettings = nullptr;
		winrt::Windows::Storage::StorageFolder				m_roamingFolder = nullptr;

		bool												m_roamingChanged;
	};

	class Storage
	{
	public:
		Storage();

		// Write
		void WriteLocalSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _In_ winrt::Windows::Foundation::IInspectable const& setting);
		void WriteRoamingSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _In_ winrt::Windows::Foundation::IInspectable const& setting);

		void WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text);
		void WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer);
		void WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer);

		void WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text);
		void WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer);
		void WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer);

		void WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text);
		void WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer);
		void WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer);

		// Read
		bool ReadLocalSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _Out_ winrt::Windows::Foundation::IInspectable setting);
		bool ReadRoamingSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _Out_ winrt::Windows::Foundation::IInspectable setting);

		bool ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring text);
		bool ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer);
		bool ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer);

		bool ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring  text);
		bool ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer);
		bool ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer);

		bool ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring text);
		bool ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer);
		bool ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer);

		bool RoaminghasChanged();

	private:
		WOtech::RoamingDataHandler* m_roamingHandler;

		winrt::Windows::Storage::ApplicationDataContainer	m_localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();
		winrt::Windows::Storage::StorageFolder				m_localFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
		winrt::Windows::Storage::ApplicationDataContainer	m_roamingSettings = winrt::Windows::Storage::ApplicationData::Current().RoamingSettings();
		winrt::Windows::Storage::StorageFolder				m_roamingFolder = winrt::Windows::Storage::ApplicationData::Current().RoamingFolder();

		winrt::Windows::Storage::StorageFolder				m_temporaryFolder = winrt::Windows::Storage::ApplicationData::Current().TemporaryFolder();

		bool												m_roamingChanged;
	};
}
#endif