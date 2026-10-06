#ifndef WO_FONTFILELOADER_H
#define WO_FONTFILELOADER_H

#include "..\WO_pch.hpp"
#include <dwrite.h>

namespace WOtech
{
	HRESULT LoadFontFromFile(_In_ winrt::hstring const& pFilename, _In_ UINT const& fontId, _In_ IDWriteFactory* const& writeFactory, _Out_ IDWriteFontCollection** pFontCollection);

	HRESULT LoadFontFromFile(_In_ winrt::hstring const& pFilename, _In_ winrt::hstring const& pFontname, _In_ UINT const& fontId, _In_ FLOAT const& fontSize, _In_ IDWriteFactory* const& writeFactory, _Out_ IDWriteTextFormat** pTextFormat);

	class FileFontCollectionLoader : public IDWriteFontCollectionLoader
	{
	public:
		FileFontCollectionLoader(_In_ winrt::hstring pFilename);

		virtual ~FileFontCollectionLoader();

		IFACEMETHOD_(unsigned long, AddRef) ();

		IFACEMETHOD_(unsigned long, Release) ();

		IFACEMETHOD(QueryInterface) (_In_ IID const& riid, _Out_ void** ppvObject);

		IFACEMETHOD(CreateEnumeratorFromKey)(_In_ IDWriteFactory* factory, _In_ void const* collectionKey, _In_ UINT collectionKeySize, _Out_ IDWriteFontFileEnumerator** fontFileEnumerator);

	private:
		LONG cRefCount_;
		winrt::hstring filename_;
	};

	class FileFontFileEnumerator : public IDWriteFontFileEnumerator
	{
	public:
		FileFontFileEnumerator(_In_ IDWriteFactory* factory, _In_ winrt::hstring pFilename);

		virtual ~FileFontFileEnumerator();

		IFACEMETHOD_(unsigned long, AddRef) ();

		IFACEMETHOD_(unsigned long, Release) ();

		IFACEMETHOD(QueryInterface) (_In_ IID const& riid, _Out_ void** ppvObject);

		IFACEMETHOD(MoveNext) (_Out_ BOOL* hasCurrentFile);

		IFACEMETHOD(GetCurrentFontFile) (_Out_ IDWriteFontFile** fontFile);

	private:
		LONG cRefCount_;
		Microsoft::WRL::ComPtr<IDWriteFactory> factory_;
		Microsoft::WRL::ComPtr<IDWriteFontFile> currentFile_;
		winrt::hstring filename_;
		BOOL isLoaded_;
	};
}
#endif
