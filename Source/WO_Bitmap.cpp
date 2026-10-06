////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Bitmap.cpp
///
///			Description:
///
///			Created:	20.10.2017
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Utilities.hpp"
#include "WO_2DComponents.hpp"
#include "WO_SpriteBatch.hpp"

namespace WOtech
{
	Bitmap::Bitmap()
	{
		m_bitmap = nullptr;
	}

	Bitmap::Bitmap(_In_ WOtech::SpriteBatch* const& spriteBatch, _In_ UINT const& width, _In_ UINT const& height)
	{
		auto size = D2D1::SizeU(width, height);
		auto properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

		spriteBatch->GetDeviceContext()->CreateBitmap(size, nullptr, 0, &properties, m_bitmap.ReleaseAndGetAddressOf());
	}

	Bitmap::Bitmap(_In_ WOtech::SpriteBatch* const& spriteBatch, _In_  winrt::hstring const& fileName)
	{
		m_bitmap = (spriteBatch->LoadBitmap(fileName))->getBitmap();
	}

	void Bitmap::Reset()
	{
		m_bitmap.Reset();
	}

	const D2D1_SIZE_F Bitmap::getSize()
	{
		auto size = m_bitmap->GetSize();

		return D2D1_SIZE_F(size.width, size.height);
	}

	const D2D1_RECT_F Bitmap::getSourceRECT()
	{
		auto size = m_bitmap->GetSize();
		return D2D1_RECT_F{ 0.0f, 0.0f, size.width, size.height };
	}

	Bitmap::Bitmap(_In_ ID2D1Bitmap1* const& bitmap)
	{
		m_bitmap = bitmap;
	}

	void Bitmap::setBitmap(_In_ ID2D1Bitmap1* const& bitmap)
	{
		m_bitmap = bitmap;
	}
	ID2D1Bitmap1* Bitmap::getBitmap()
	{
		return m_bitmap.Get();
	}
	Bitmap::~Bitmap()
	{
	}
}