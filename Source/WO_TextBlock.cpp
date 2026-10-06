////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: TextBlock.cpp
///
///			Description:
///
///			Created:	05.09.2014
///			Edited:		01.06.2018
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
	void TextBlock::CreateText(_In_ WOtech::Font* const& font, _In_ WOtech::SpriteBatch* const& spriteBatch)
	{
		m_font = font;

		m_size = 16.0f;
		m_text = L"This is a Text";
		m_style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		m_position = { 0.0f, 0.0f };
		m_layoutbox = { 800.0f, 600.0f };

		makeBrush(spriteBatch);
		makeText();

		// todo: Content Manager
	}
	void TextBlock::CreateText(_In_ WOtech::Font* const& font, _In_  WOtech::SpriteBatch* const& spriteBatch, _In_ FLOAT const& size, _In_ winrt::hstring const& text, _In_ DWRITE_FONT_STYLE const& style, _In_ D2D1_COLOR_F const& color, _In_ D2D1_POINT_2F const& position, _In_ D2D1_SIZE_F const& layoutsize)
	{
		m_font = font;

		m_size = size;
		m_text = text;
		m_style = style;
		m_position = position;
		m_layoutbox = layoutsize;

		makeBrush(spriteBatch);

		m_brush->SetColor(color);

		makeText();
	}

	void TextBlock::setSize(_In_ FLOAT const& size)
	{
		if (size != m_size)
		{
			m_size = size;

			makeText();
		}
	}

	void TextBlock::setText(_In_ winrt::hstring const& text)
	{
		if (text != m_text)
		{
			m_text = text;

			makeText();
		}
	}

	void TextBlock::setPosition(_In_ FLOAT const& x, _In_ FLOAT const& y)
	{
		m_position.x = x;
		m_position.y = y;
	}
	void TextBlock::setPosition(_In_ D2D1_POINT_2F const& position)
	{
		m_position = position;
	}

	void TextBlock::setRotation(_In_ FLOAT const& degree)
	{
		m_rotation = degree;
	}

	void TextBlock::setRotationinRadian(_In_ FLOAT const& radian)
	{
		m_rotation = RadiantoDegree(radian);
	}

	void TextBlock::setRotationinVector(_In_ FLOAT const& x, _In_ FLOAT const& y)
	{
		m_rotation = VectortoDegree(x, y);
	}

	void TextBlock::setColor(_In_ FLOAT const& r, _In_ FLOAT const& g, _In_ FLOAT const& b, _In_ FLOAT const& a)
	{
		if (m_brush)
		{
			m_brush->SetColor(D2D1::ColorF(r, g, b, a));
		}
	}
	void TextBlock::setColor(_In_ D2D1_COLOR_F const& color)
	{
		if (m_brush)
		{
			m_brush->SetColor(color);
		}
	}

	void TextBlock::setStyle(_In_ DWRITE_FONT_STYLE const& style)
	{
		if (style != m_style)
		{
			m_style = style;

			makeText();
		}
	}

	void TextBlock::setLayoutBox(_In_ FLOAT const& w, _In_ FLOAT const& h)
	{
		if (w != m_layoutbox.width && h != m_layoutbox.height)
		{
			m_layoutbox.width = w;
			m_layoutbox.height = h;

			makeText();
		}
	}
	void TextBlock::setLayoutBox(_In_ D2D1_SIZE_F const& layoutboxsize)
	{
		if (layoutboxsize.width != m_layoutbox.width && layoutboxsize.height != m_layoutbox.height)
		{
			m_layoutbox = layoutboxsize;

			makeText();
		}
	}

	TextBlock::~TextBlock()
	{
		// todo: Content Manager
	}

	void TextBlock::makeText()
	{
		HRESULT hr;

		if (!m_font->getColletion()) //todo: make it nice
			ThrowIfFailed(E_FAIL);

		m_layout = nullptr;
		m_format = nullptr;

		if (m_wFactory == NULL)
		{
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_wFactory);
			ThrowIfFailed(hr);
		}

		hr = m_wFactory->CreateTextFormat(m_font->getFontname().data(), m_font->getColletion(), DWRITE_FONT_WEIGHT_NORMAL, m_style, DWRITE_FONT_STRETCH_NORMAL, m_size, L"", &m_format);
		ThrowIfFailed(hr);

		hr = m_wFactory->CreateTextLayout(m_text.data(), static_cast<UINT>(m_text.size()), m_format.Get(), m_layoutbox.width, m_layoutbox.height, &m_layout);
		ThrowIfFailed(hr);
	} //todo: memory leak? ->growing memory by calling it often

	void TextBlock::makeBrush(_In_ WOtech::SpriteBatch* const& spriteBatch)
	{
		HRESULT hr;

		if (!spriteBatch)
		{
			ThrowIfFailed(E_FAIL);
		}

		hr = spriteBatch->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &m_brush);
		ThrowIfFailed(hr);
	}

	IDWriteTextLayout* TextBlock::getLayout()
	{
		return m_layout.Get();
	}
	ID2D1SolidColorBrush* TextBlock::getBrush()
	{
		return m_brush.Get();
	};
	D2D1_POINT_2F TextBlock::getPosition()
	{
		return m_position;
	};
	FLOAT TextBlock::getRotation()
	{
		return m_rotation;
	}
	D2D1_SIZE_F TextBlock::getlayoutbox()
	{
		return m_layoutbox;
	}
}//namespace WOtech