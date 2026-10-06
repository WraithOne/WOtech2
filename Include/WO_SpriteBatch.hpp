////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: SpriteBatch.cpp
///
///			Description:
///
///			Created:	07.05.2014
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_SPRITEBATCH_H
#define WO_SPRITEBATCH_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "..\Include\WO_2DComponents.hpp"

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#undef LoadBitmap

namespace WOtech
{
	// Forward Declaration
	class DeviceDX11;

	enum SPRITE_SORT_MODE
	{
		SPRITE_SORT_MODE_IMMEDIATE,
		SPRITE_SORT_MODE_DEFERRED,
		SPRITE_SORT_MODE_TEXTURE,
		SPRITE_SORT_MODE_BACKTOFRONT,
		SPRITE_SORT_MODE_FRONTTOBACK
	};

	class SpriteBatch
	{
	public:
		SpriteBatch(_In_ WOtech::DeviceDX11* const& device);
		~SpriteBatch();

		void Initialize();

		void BeginDraw();
		void BeginDraw(_In_ WOtech::SPRITE_SORT_MODE const& sortmode);

		void EndDraw();

		void DrawTextBlock(_In_ WOtech::TextBlock* const& text);
		void DrawString(_In_ WOtech::Font* const& font, _In_ FLOAT const& fontSize, _In_ D2D1_RECT_F const& layoutbox, _In_ DWRITE_FONT_STYLE const& style, _In_ D2D1_COLOR_F const& color, _In_ FLOAT const& rotation, _In_ winrt::hstring const& text);

		void DrawBitmap(_In_ WOtech::Bitmap* const& bitmap);
		void DrawBitmap(_In_ WOtech::Bitmap* const& bitmap, _In_ D2D1_RECT_F const& srcRect, _In_ D2D1_RECT_F const& destRect, _In_ FLOAT const& opacity, _In_ FLOAT const& rotation);

		void DrawSprite(_In_ WOtech::Sprite* const& sprite);
		void DrawSprite(_In_ WOtech::Sprite* const& sprite, _In_ D2D1_RECT_F const& srcRect, _In_ D2D1_RECT_F const& destRect, _In_ FLOAT const& opacity, _In_ FLOAT const& rotation, _In_ WOtech::SPRITE_FLIP_MODE const& flipmode);

		void DrawAnimatedSprite(_In_ WOtech::AnimatedSprite* const& animatedsprite, _In_ winrt::hstring const& name);

		void DrawGrid(_In_ D2D1_RECT_F const& area, _In_ D2D1_COLOR_F const& color, _In_ FLOAT const& rotation);

		void DrawCircle(_In_ WOtech::Circle const& circle);
		void DrawCircleOutlined(_In_ WOtech::CircleOutlined const& circleOutlined);
		void DrawCircleFilled(_In_ WOtech::CircleFilled const& circleFilled);

		void DrawRectangle(_In_ WOtech::Rectangle const& rectangle);
		void DrawRectangleOutlined(_In_ WOtech::RectangleOutlined const& rectangleOutlined);
		void DrawRectangleFilled(_In_ WOtech::RectangleFilled const& rectangleFilled);

		void DrawGeometry(_In_ WOtech::Geometry* const& geometry, _In_ FLOAT const& strokeWidth);

		// Getter
		WOtech::Image*	getRenderTarget();
		D2D1_SIZE_F		getLogicalSize();
		D2D1_RECT_F		getRenderRect();

		ID2D1DeviceContext5*	GetDeviceContext();
		ID2D1Factory6*			getFactory();

		// Setter
		void setRenderTarget(_In_ WOtech::Image* const& rendertarget);
		void setRenderTarget(_In_ WOtech::Bitmap* const& rendertarget);

		WOtech::Bitmap* LoadBitmap(_In_ winrt::hstring const& fileName);

		void ReleaseRendertarget();

	private:
		void CreateGrid(_In_ D2D1_COLOR_F const& color);

		void setRotation(_In_ D2D1_RECT_F const& area, _In_ FLOAT const& rotation);
		void setTransformation(_In_ WOtech::SPRITE_FLIP_MODE const& flipMode);

		void SortBatch();
		void DrawBatch();

		void Release();

	private:
		WOtech::DeviceDX11* m_deviceDX11;

		Microsoft::WRL::ComPtr<ID2D1Factory6>			m_factory;
		Microsoft::WRL::ComPtr<ID2D1Device5>			m_device;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext5>		m_deviceContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>			m_targetBitmap;

		Microsoft::WRL::ComPtr<IWICImagingFactory>		m_wicFactory;
		Microsoft::WRL::ComPtr<IDWriteFactory5>			m_dwFactory;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_fontBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_circleBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_rectangleBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_outlineBrush;
		Microsoft::WRL::ComPtr<ID2D1BitmapBrush1>		m_gridBrush;
		D2D1_COLOR_F									m_gridColor;

		WOtech::SPRITE_SORT_MODE						m_sortMode;
		bool											m_beginDraw;
	};// class SpriteBatch
}
#endif