////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Storage.cpp
///
///			Description:
///
///			Created:	09.01.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Storage.hpp"

namespace WOtech
{
	Storage::Storage()
	{
		m_roamingHandler = new RoamingDataHandler();

		m_roamingChanged = false;
	}

	void Storage::WriteLocalSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _In_ winrt::Windows::Foundation::IInspectable const& setting)
	{
		m_localSettings.CreateContainer(containerName, winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);

		if (m_localSettings.Containers().HasKey(containerName))
		{
			auto values = m_localSettings.Containers().Lookup(containerName).Values();
			values.Insert(settingName, setting);
		}
	}
	void Storage::WriteRoamingSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _In_ winrt::Windows::Foundation::IInspectable const& setting)
	{
		m_roamingSettings.CreateContainer(containerName, winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);

		if (m_roamingSettings.Containers().HasKey(containerName))
		{
			auto values = m_roamingSettings.Containers().Lookup(containerName).Values();
			values.Insert(settingName, setting);
		}
	}

	void Storage::WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return;
	}
	void Storage::WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}
	void Storage::WriteLocalFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}

	void Storage::WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return;
	}
	void Storage::WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}
	void Storage::WriteRoamingFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}

	void Storage::WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ winrt::hstring const& text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return;
	}
	void Storage::WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ BYTE* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}
	void Storage::WriteTemporaryFile(_In_ winrt::hstring const& fileName, _In_ winrt::Windows::Storage::Streams::IBuffer* const& buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return;
	}

	bool Storage::ReadLocalSetting(_In_ winrt::hstring const& containerName, _In_ winrt::hstring const& settingName, _Out_ winrt::Windows::Foundation::IInspectable setting)
	{
		bool hasSetting = false;

		if (m_localSettings.Containers().HasKey(containerName))
		{
			if (m_localSettings.Containers().Lookup(containerName).Values().HasKey(settingName))
			{
				setting = m_localSettings.Containers().Lookup(containerName).Values().Lookup(settingName);
				hasSetting = true;
			}
		}
		return hasSetting;
	}
	bool Storage::ReadRoamingSetting(_In_ winrt::hstring const& containerName, _In_  winrt::hstring const& settingName, _Out_ winrt::Windows::Foundation::IInspectable setting)
	{
		bool hasSetting = false;

		if (m_roamingSettings.Containers().HasKey(containerName))
		{
			if (m_roamingSettings.Containers().Lookup(containerName).Values().HasKey(settingName))
			{
				setting = m_localSettings.Containers().Lookup(containerName).Values().Lookup(settingName);
				hasSetting = true;
			}
		}
		return hasSetting;
	}

	bool Storage::ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return false;
	}
	bool Storage::ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		buffer = nullptr;

		return false;
	}
	bool Storage::ReadLocalFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(buffer);

		return false;
	}

	bool Storage::ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return false;
	}
	bool Storage::ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		buffer = nullptr;

		return false;
	}
	bool Storage::ReadRoamingFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		buffer = nullptr;

		return false;
	}

	bool Storage::ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ winrt::hstring text)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(text);

		return false;
	}
	bool Storage::ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ BYTE* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		buffer = nullptr;

		return false;
	}
	bool Storage::ReadTemporaryFile(_In_ winrt::hstring const& fileName, _Out_ winrt::Windows::Storage::Streams::IBuffer* buffer)
	{
		UNREFERENCED_PARAMETER(fileName);
		buffer = nullptr;

		return false;
	}

	bool Storage::RoaminghasChanged()
	{
		if (m_roamingHandler->isRoamingChanged())
		{
			m_roamingHandler->setRoamingChanged(false);
			m_roamingSettings = m_roamingHandler->getRoamingSettings();
			m_roamingFolder = m_roamingHandler->getRoamingFolder();

			return true;
		}
		return false;
	}

	RoamingDataHandler::RoamingDataHandler()
	{
		winrt::Windows::Storage::ApplicationData::Current().DataChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Storage::ApplicationData, winrt::Windows::Foundation::IInspectable>(this, &WOtech::RoamingDataHandler::DataChangeHandler));
	}

	void RoamingDataHandler::DataChangeHandler(_In_ winrt::Windows::Storage::ApplicationData const& appData, _In_ winrt::Windows::Foundation::IInspectable const& o)
	{
		UNREFERENCED_PARAMETER(o);

		m_roamingSettings = appData.Current().RoamingSettings();
		m_roamingFolder = appData.Current().RoamingFolder();

		m_roamingChanged = true;
	}
}