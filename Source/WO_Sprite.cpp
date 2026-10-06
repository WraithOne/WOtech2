////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Sprite.cpp
///
///			Description:
///
///			Created:	07.05.2014
///			Edited:		28.08.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_2DComponents.hpp"
#include "WO_SpriteBatch.hpp"
#include "WO_Utilities.hpp"

namespace WOtech
{
	Sprite::Sprite(_In_ winrt::hstring const& filename)
	{
		m_fileName = filename;

		// Set default attributes
		m_position = { 0.0f,0.0f };
		m_size = { 0.0f,0.0f };
		m_sourceRect = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_rotation = 0.0f;
		m_opacity = 1.0f;
		m_interpolation = D2D1_INTERPOLATION_MODE::D2D1_INTERPOLATION_MODE_LINEAR;
		m_flipMode = WOtech::SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_NONE;
	}
	void Sprite::Load(_In_ WOtech::SpriteBatch* const& spriteBatch)
	{
		m_bitmap = (spriteBatch->LoadBitmap(m_fileName))->getBitmap();

		// Set width/height of the source rect
		m_size.width = m_sourceRect.right = m_bitmap->GetSize().width;
		m_size.height = m_sourceRect.bottom = m_bitmap->GetSize().height;
	}

	void Sprite::UnLoad()
	{
		m_bitmap.Reset();
	}

	ID2D1Bitmap1* Sprite::getBitmap()
	{
		return m_bitmap.Get();
	}

	Sprite::~Sprite()
	{
	}

	D2D1_RECT_F Sprite::getSourceRect()
	{
		return m_sourceRect;
	}
	D2D1_RECT_F Sprite::getDestinationRect()
	{
		return { m_position.x, m_position.y, m_position.x + m_size.width, m_position.y + m_size.height };
	}
	D2D1_POINT_2F Sprite::getPosition()
	{
		return m_position;
	}
	D2D1_SIZE_F Sprite::getSize()
	{
		return m_size;
	}
	FLOAT Sprite::getOpacity()
	{
		return m_opacity;
	}
	FLOAT Sprite::getRotation()
	{
		return m_rotation;
	}
	SPRITE_FLIP_MODE Sprite::getFlipMode()
	{
		return m_flipMode;
	}
	D2D1_INTERPOLATION_MODE Sprite::getInterpolation()
	{
		return m_interpolation;
	}

	void Sprite::setPosition(_In_ FLOAT const& x, _In_ FLOAT const& y)
	{
		m_position = D2D1_POINT_2F(x, y);
	}
	void Sprite::setSize(_In_ FLOAT const& width, _In_ FLOAT const& height)
	{
		m_size = D2D1_SIZE_F(width, height);
	}
	void Sprite::setSourceRect(_In_ FLOAT const& x, _In_ FLOAT const& y, _In_ FLOAT const& height, _In_ FLOAT const& width)
	{
		m_sourceRect = D2D1_RECT_F{ x, y, width, height };
	}
	void Sprite::setOpacity(_In_ FLOAT const& opacity)
	{
		m_opacity = opacity;
	}
	void Sprite::setRotation(_In_ FLOAT const& degree)
	{
		m_rotation = degree;
	}
	void Sprite::setRotationinRadian(_In_ FLOAT const& radian)
	{
		m_rotation = RadiantoDegree(radian);
	}

	void Sprite::setFlipMode(_In_ WOtech::SPRITE_FLIP_MODE const& flipmode)
	{
		m_flipMode = flipmode;
	}

	void Sprite::setInterpolation(_In_ D2D1_INTERPOLATION_MODE const& interpolation)
	{
		m_interpolation = interpolation;
	}
}//namespace WOtech