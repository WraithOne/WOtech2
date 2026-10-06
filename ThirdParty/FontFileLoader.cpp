#include "WO_pch.hpp"
#include "FontFileLoader.hpp"

namespace WOtech
{
	HRESULT LoadFontFromFile(_In_ winrt::hstring const& pFilename, _In_ UINT const& fontId, _In_ IDWriteFactory* const& writeFactory, _Out_ IDWriteFontCollection** pFontCollection)
	{
		Microsoft::WRL::ComPtr<IDWriteFontCollectionLoader> pFontCollectionLoader(new(std::nothrow) FileFontCollectionLoader(pFilename));

		HRESULT hr = writeFactory->RegisterFontCollectionLoader(pFontCollectionLoader.Get());

		if (SUCCEEDED(hr))
		{
			hr = writeFactory->CreateCustomFontCollection(pFontCollectionLoader.Get(), &fontId, sizeof(fontId), pFontCollection);

			if (FAILED(hr))
			{
				writeFactory->UnregisterFontCollectionLoader(pFontCollectionLoader.Get());
				return hr;
			}
		}

		hr = writeFactory->UnregisterFontCollectionLoader(pFontCollectionLoader.Get());

		return hr;
	}

	HRESULT LoadFontFromFile(_In_ winrt::hstring const& pFilename, _In_ winrt::hstring const& pFontname, _In_ UINT const& fontId, _In_ FLOAT const& fontSize, _In_ IDWriteFactory* const& writeFactory, _Out_ IDWriteTextFormat** pTextFormat)
	{
		Microsoft::WRL::ComPtr<IDWriteFontCollection> pFontCollection;

		HRESULT hr = WOtech::LoadFontFromFile(pFilename, fontId, writeFactory, pFontCollection.ReleaseAndGetAddressOf());

		if (SUCCEEDED(hr))
		{
			hr = writeFactory->CreateTextFormat(pFontname.data(), pFontCollection.Get(), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-us", pTextFormat);
		}

		return hr;
	}

	FileFontCollectionLoader::FileFontCollectionLoader(_In_ winrt::hstring pFilename) : cRefCount_(0), filename_(pFilename)
	{
	}

	FileFontCollectionLoader::~FileFontCollectionLoader()
	{
	}

	IFACEMETHODIMP_(unsigned long) FileFontCollectionLoader::AddRef()
	{
		return ::InterlockedIncrement(&cRefCount_);
	}

	IFACEMETHODIMP_(unsigned long) FileFontCollectionLoader::Release()
	{
		unsigned long newCount = ::InterlockedDecrement(&cRefCount_);
		if (newCount == 0)
		{
			delete this;
			return 0;
		}

		return newCount;
	}

	IFACEMETHODIMP FileFontCollectionLoader::QueryInterface(_In_ IID const& riid, _Out_ void** ppvObject)
	{
		if ((riid == IID_IUnknown) || (riid == __uuidof(IDWriteFontCollectionLoader)))
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		this->AddRef();

		return S_OK;
	}

	IFACEMETHODIMP FileFontCollectionLoader::CreateEnumeratorFromKey(_In_ IDWriteFactory* factory, _In_ void const* collectionKey, _In_ UINT collectionKeySize, _Out_  IDWriteFontFileEnumerator** fontFileEnumerator)
	{
		UNREFERENCED_PARAMETER(collectionKeySize);
		UNREFERENCED_PARAMETER(collectionKey);

		*fontFileEnumerator = NULL;

		IDWriteFontFileEnumerator* pEnumerator = new(std::nothrow)FileFontFileEnumerator(factory, filename_);

		HRESULT hr = ((pEnumerator != NULL) ? S_OK : E_OUTOFMEMORY);

		if (SUCCEEDED(hr))
		{
			pEnumerator->AddRef();
			*fontFileEnumerator = pEnumerator;
		}

		return hr;
	}

	FileFontFileEnumerator::FileFontFileEnumerator(_In_ IDWriteFactory* factory, _In_ winrt::hstring pFilename) : cRefCount_(0), factory_(factory), filename_(pFilename), isLoaded_(false)
	{
	}

	FileFontFileEnumerator::~FileFontFileEnumerator()
	{
	}

	IFACEMETHODIMP_(unsigned long) FileFontFileEnumerator::AddRef()
	{
		return ::InterlockedIncrement(&cRefCount_);
	}

	IFACEMETHODIMP_(unsigned long) FileFontFileEnumerator::Release()
	{
		unsigned long newCount = ::InterlockedDecrement(&cRefCount_);
		if (newCount == 0)
		{
			delete this;
			return 0;
		}

		return newCount;
	}

	IFACEMETHODIMP FileFontFileEnumerator::QueryInterface(_In_ IID const& riid, _Out_ void** ppvObject)
	{
		if ((riid == IID_IUnknown) || (riid == __uuidof(IDWriteFontFileEnumerator)))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else {
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP FileFontFileEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
	{
		HRESULT hr = S_OK;

		*hasCurrentFile = FALSE;
		currentFile_.Reset();

		if (!isLoaded_)
		{
			hr = factory_->CreateFontFileReference(filename_.data(), NULL, &currentFile_);
			if (SUCCEEDED(hr))
			{
				*hasCurrentFile = TRUE;
				isLoaded_ = true;
			}
		}

		return hr;
	}

	IFACEMETHODIMP FileFontFileEnumerator::GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile)
	{
		if (currentFile_.Get() != NULL)
		{
			currentFile_.Get()->AddRef();
		}
		*fontFile = ((currentFile_.Get() != NULL) ? currentFile_.Get() : NULL);

		return (currentFile_.Get() != NULL) ? S_OK : E_FAIL;
	}
}