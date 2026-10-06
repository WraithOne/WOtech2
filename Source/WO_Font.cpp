////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Font.cpp
///
///			Description:
///
///			Created:	31.08.2014
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_2DComponents.hpp"
#include "FontFileLoader.hpp"
#include "WO_Utilities.hpp"

namespace WOtech
{
	Font::Font(_In_ winrt::hstring const& filename)
	{
		m_fileName = filename;
	}

	void Font::Load()
	{
		HRESULT hr;
		UINT key = 150;

		Microsoft::WRL::ComPtr<IDWriteFactory> p_Wfactory;

		// Create DWriteFactory
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &p_Wfactory);
		ThrowIfFailed(hr);

		// Create Path(Filename String
		winrt::hstring path;
		winrt::hstring pathfilename;
		auto installedLocation = winrt::Windows::ApplicationModel::Package::Current().InstalledLocation();
		path = installedLocation.Path() + L"\\";
		pathfilename = path + m_fileName;

		// Load
		hr = LoadFontFromFile(pathfilename, key, p_Wfactory.Get(), &m_collection);
		ThrowIfFailed(hr);
	}

	void Font::UnLoad()
	{
		m_collection.Reset();
	}

	IDWriteFontCollection* Font::getColletion()
	{
		return m_collection.Get();
	}
	IDWriteFontCollection** Font::getCollectionL()
	{
		return m_collection.GetAddressOf();
	}

	Font::~Font()
	{
	}

	winrt::hstring Font::getFontname()
	{
		if (m_collection != NULL)
		{
			Microsoft::WRL::ComPtr<IDWriteFontFamily> pFontFamily;
			Microsoft::WRL::ComPtr<IDWriteLocalizedStrings> pFamilyNames;

			m_collection->GetFontFamily(0, &pFontFamily);

			pFontFamily->GetFamilyNames(&pFamilyNames);

			UINT length = 0;
			pFamilyNames->GetStringLength(0, &length);

			wchar_t* name = new wchar_t[length + 1];
			pFamilyNames->GetString(0, name, length + 1);

			winrt::hstring string = name;
			delete[] name;

			return string;
		}

		return L"Failed";
	}
}//namespace WOtech