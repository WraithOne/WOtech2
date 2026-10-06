////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Image.cpp
///
///			Description:
///
///			Created:	20.10.2017
///			Edited:		25.12.2017
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_2DComponents.hpp"
#include "WO_SpriteBatch.hpp"

namespace WOtech
{
	Image::Image()
	{
		m_image = nullptr;
	}

	void Image::Reset()
	{
		m_image.Reset();;
	}

	Image::Image(_In_ ID2D1Image* const& image)
	{
		m_image = image;
	}

	void Image::setImage(_In_ ID2D1Image* const& image)
	{
		m_image = image;
	}

	ID2D1Image* Image::getImage()
	{
		return m_image.Get();
	}
	Image::~Image()
	{
	}
}