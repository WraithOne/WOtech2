////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: AnimatedSprite.cpp
///
///			Description:
///
///			Created:	13.09.2014
///			Edited:		23.09.2018
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
	AnimatedSprite::AnimatedSprite(_In_ winrt::hstring const& filename)
	{
		m_fromFile = true;
		m_fileName = filename;
		m_position = D2D1_POINT_2F(0.0f, 0.0f);
		m_scale = 1.0f;
		m_rotation = 0.0f;
		m_opacity = 1.0f;
		m_interpolation = D2D1_INTERPOLATION_MODE::D2D1_INTERPOLATION_MODE_LINEAR;
		m_flipMode = SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_NONE;
	}

	AnimatedSprite::AnimatedSprite(_In_ WOtech::Bitmap* const& bitmap)
	{
		m_fromFile = true;
		m_fileName = L"Error not loaded from File";
		m_position = D2D1_POINT_2F(0.0f, 0.0f);
		m_scale = 1.0f;
		m_rotation = 0.0f;
		m_opacity = 1.0f;
		m_interpolation = D2D1_INTERPOLATION_MODE::D2D1_INTERPOLATION_MODE_LINEAR;
		m_flipMode = SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_NONE;

		m_bitmap = bitmap->getBitmap();
	}

	void AnimatedSprite::Load(_In_ WOtech::SpriteBatch* const& spriteBatch)
	{
		if (m_fromFile)
		{
			m_bitmap = (spriteBatch->LoadBitmap(m_fileName))->getBitmap();
		}
	}
	void AnimatedSprite::UnLoad()
	{
		m_bitmap.Reset();
	}
	bool AnimatedSprite::AddAnimation(_In_ winrt::hstring const& name, _In_ UINT const& framecount, _In_ FLOAT const& frametime, _In_ D2D1_SIZE_F const& framesize, _In_ D2D1_POINT_2F const& sourceposition)
	{
		std::list<WOtech::Animation>::iterator iterator;

		for (iterator = m_animationList.begin(); iterator != m_animationList.end(); ++iterator)
		{
			if (iterator->Name == name)
			{
				//g_pLogfile->Textout("Animation exists");
				return false;
			}
		}
		WOtech::Animation temp;

		temp.ActualFrame = 0;
		temp.Framecount = framecount;
		temp.FrameSize = framesize;
		temp.Frametime = frametime;
		temp.Lastime = 0.0f;
		temp.Name = name;
		temp.SourcePosition = sourceposition;

		m_animationList.push_back(temp);

		return true;
	}
	void AnimatedSprite::Update(_In_ winrt::hstring const& name, _In_ FLOAT const& elapsed)
	{
		std::list<WOtech::Animation>::iterator iterator;

		for (iterator = m_animationList.begin(); iterator != m_animationList.end(); ++iterator)
		{
			if (iterator->Name == name)
			{
				iterator->Lastime += elapsed;
				if (iterator->Lastime > iterator->Frametime)
				{
					iterator->ActualFrame++;
					iterator->Lastime = elapsed;

					if (iterator->ActualFrame > iterator->Framecount)
					{
						iterator->ActualFrame = 0;
						iterator->Lastime = 0.0f;
					}
				}
			}
		}
	}
	void AnimatedSprite::Restart(_In_ winrt::hstring const& name)
	{
		std::list<WOtech::Animation>::iterator iterator;

		for (iterator = m_animationList.begin(); iterator != m_animationList.end(); ++iterator)
		{
			if (iterator->Name == name)
			{
				iterator->ActualFrame = 0;
				iterator->Lastime = 0.0f;
			}
		}
	}

	ID2D1Bitmap* AnimatedSprite::getBitmap()
	{
		return m_bitmap.Get();
	}
	AnimatedSprite::~AnimatedSprite()
	{
	}
	D2D1_POINT_2F AnimatedSprite::getPosition()
	{
		return m_position;
	}
	FLOAT AnimatedSprite::getScale()
	{
		return m_scale;
	}
	D2D1_SIZE_F AnimatedSprite::getFrameSize(_In_ winrt::hstring const& name)
	{
		std::list<WOtech::Animation>::iterator iterator;

		for (iterator = m_animationList.begin(); iterator != m_animationList.end(); ++iterator)
		{
			if (iterator->Name == name)
			{
				return iterator->FrameSize;
			}
		}
		D2D1_SIZE_F temp;
		temp.width = 0.0f;
		temp.height = 0.0f;

		return temp;
	}

	D2D1_RECT_F AnimatedSprite::getFrame(_In_ winrt::hstring const& name)
	{
		std::list<WOtech::Animation>::iterator iterator;

		for (iterator = m_animationList.begin(); iterator != m_animationList.end(); ++iterator)
		{
			if (iterator->Name == name)
			{
				D2D1_RECT_F temp;
				WOtech::Animation animation = *iterator;

				FLOAT left = animation.SourcePosition.x + (animation.FrameSize.width * animation.ActualFrame);
				FLOAT top = animation.SourcePosition.y;
				temp.left = left;
				temp.top = top;
				temp.right = left + animation.FrameSize.width;
				temp.bottom = top + animation.FrameSize.height;

				return temp;
			}
		}
		return D2D1_RECT_F{ 0.0f, 0.0f, 0.0f, 0.0f };
	}
	FLOAT AnimatedSprite::getOpacity()
	{
		return m_opacity;
	}
	FLOAT AnimatedSprite::getRotation()
	{
		return m_rotation;
	}
	SPRITE_FLIP_MODE AnimatedSprite::getFlipMode()
	{
		return m_flipMode;
	}
	D2D1_INTERPOLATION_MODE AnimatedSprite::getInterpolation()
	{
		return m_interpolation;
	}

	void AnimatedSprite::setPosition(_In_ FLOAT const& x, _In_  FLOAT const& y)
	{
		m_position = { x,y };
	}
	void AnimatedSprite::setScale(_In_ FLOAT const& scale)
	{
		m_scale = scale;
	}
	void AnimatedSprite::setOpacity(_In_ FLOAT const& opacity)
	{
		m_opacity = opacity;
	}
	void AnimatedSprite::setRotation(_In_ FLOAT const& degree)
	{
		m_rotation = degree;
	}
	void AnimatedSprite::setRotationinRadian(_In_ FLOAT const& radian)
	{
		m_rotation = WOtech::RadiantoDegree(radian);
	}
	void AnimatedSprite::setFlipMode(_In_ SPRITE_FLIP_MODE const& flipmode)
	{
		m_flipMode = flipmode;
	}
	void AnimatedSprite::setInterpolation(_In_ D2D1_INTERPOLATION_MODE const& interpolation)
	{
		m_interpolation = interpolation;
	}
}//namespace WOtech