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
///			Edited:		23.09.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "Include\WO_SpriteBatch.hpp"
#include "Include\WO_DeviceDX11.hpp"
#include "Include\WO_Utilities.hpp"

namespace WOtech
{
	///////////////////////////////////////////////////////////////
	// SpriteBatch
	///////////////////////////////////////////////////////////////
	SpriteBatch::SpriteBatch(_In_ WOtech::DeviceDX11* const& device)
	{
		m_deviceDX11 = device;
		m_beginDraw = false;
	}

	void SpriteBatch::Initialize()
	{
		if (m_deviceDX11 == nullptr)
		{
			WOtech::ThrowIfFailed(E_FAIL);
		}

		HRESULT hr;

		// Initialize Direct2D resources.
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		// Create the Factory
		if (m_factory == nullptr)
		{
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory6), &options, &m_factory);// D2D1_FACTORY_TYPE_SINGLE_THREADED
			WOtech::ThrowIfFailed(hr);
		}

		// Create the DirectWriter Factory
		if (m_dwFactory == nullptr)
		{
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), &m_dwFactory);
			WOtech::ThrowIfFailed(hr);
		}

		// Create an instance of WICFactory
		if (m_wicFactory == nullptr)
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&m_wicFactory));
			WOtech::ThrowIfFailed(hr);
		}

		// Create the D2D Device
		if (m_device == nullptr)
		{
			hr = m_factory->CreateDevice(m_deviceDX11->getDXGIDevice(), m_device.ReleaseAndGetAddressOf());
			WOtech::ThrowIfFailed(hr);
		}

		// Create the D2D DeviceContext
		if (m_deviceContext == nullptr)
		{
			hr = m_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_deviceContext.ReleaseAndGetAddressOf());
			WOtech::ThrowIfFailed(hr);
		}

		m_deviceContext->SetTarget(nullptr);
		m_targetBitmap = nullptr;

		// Create a Direct2D target bitmap associated with the
		// swap chain back buffer and set it as the current target.
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				m_deviceDX11->getDPI(),
				m_deviceDX11->getDPI());

		hr = m_deviceContext->CreateBitmapFromDxgiSurface(m_deviceDX11->getSurface(), &bitmapProperties, m_targetBitmap.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		// Set target to bitmap
		m_deviceContext->SetTarget(m_targetBitmap.Get());

		// Grayscale text anti-aliasing is recommended for all Windows Store apps.
		m_deviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

		// Set DPI
		m_deviceContext->SetDpi(m_deviceDX11->getDPI(), m_deviceDX11->getDPI());

		// Set Transformation
		m_deviceContext->SetTransform(m_deviceDX11->get2DOrientation());

		// Create Circlebrush
		hr = m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), m_circleBrush.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		// create Rectanglebrush
		hr = m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), m_rectangleBrush.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		// Create Outlinebrush
		hr = m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), m_outlineBrush.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		// Create fontbrush
		hr = m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), m_fontBrush.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		// Create Gridbrush
		CreateGrid(D2D1::ColorF(D2D1::ColorF::White, 1.0f));
	}
	void SpriteBatch::ReleaseRendertarget()
	{
		m_deviceContext->SetTarget(nullptr);
		m_targetBitmap = nullptr;
	}

	WOtech::Bitmap* SpriteBatch::LoadBitmap(_In_ winrt::hstring const& fileName)
	{
		HRESULT hr;

		Microsoft::WRL::ComPtr<ID2D1Bitmap1> output;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pSource;
		Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter;

		// Create path/filename string
		winrt::hstring path;
		winrt::hstring pathfilename;
		winrt::Windows::Storage::StorageFolder installedLocation = winrt::Windows::ApplicationModel::Package::Current().InstalledLocation();
		path = installedLocation.Path() + L"\\";
		pathfilename = path + fileName;

		hr = m_wicFactory->CreateDecoderFromFilename(pathfilename.data(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
		WOtech::ThrowIfFailed(hr);

		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
		WOtech::ThrowIfFailed(hr);

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = m_wicFactory->CreateFormatConverter(&pConverter);
		WOtech::ThrowIfFailed(hr);

		hr = pConverter->Initialize(pSource.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		WOtech::ThrowIfFailed(hr);

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = m_deviceContext->CreateBitmapFromWicBitmap(pConverter.Get(), NULL, output.ReleaseAndGetAddressOf());
		WOtech::ThrowIfFailed(hr);

		return new WOtech::Bitmap(output.Get());
	}

	void SpriteBatch::BeginDraw()
	{
		BeginDraw(WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_IMMEDIATE);
	}
	void SpriteBatch::BeginDraw(_In_ WOtech::SPRITE_SORT_MODE const& sortmode)
	{
		if (m_beginDraw)
		{
			throw winrt::hresult_invalid_argument{ L"BeginDraw was called before EndDraw" };
		}
		m_sortMode = sortmode;
		m_beginDraw = true;

		m_deviceContext->BeginDraw();
	}
	void SpriteBatch::EndDraw()
	{
		if (!m_beginDraw)
		{
			throw winrt::hresult_invalid_argument{ L"EndDraw was called before BeginDraw" };
		}

		m_beginDraw = false;

		if (m_sortMode != WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_IMMEDIATE)
		{
			SortBatch();

			DrawBatch();
		}
		HRESULT hr;

		hr = m_deviceContext->EndDraw();
		ThrowIfFailed(hr);
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::CreateGrid(_In_ D2D1_COLOR_F const& color)
	{
		HRESULT hr;

		// Create a compatible render target.
		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> pCompatibleRenderTarget;
		hr = m_deviceContext->CreateCompatibleRenderTarget(D2D1::SizeF(10.0f, 10.0f), &pCompatibleRenderTarget);
		WOtech::ThrowIfFailed(hr);

		if (SUCCEEDED(hr))
		{
			// Draw a pattern.
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pGridBrush;
			hr = pCompatibleRenderTarget->CreateSolidColorBrush(color, &pGridBrush);//D2D1::ColorF(D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f))
			WOtech::ThrowIfFailed(hr);

			if (SUCCEEDED(hr))
			{
				pCompatibleRenderTarget->BeginDraw();
				pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 10.0f, 1.0f), pGridBrush.Get());
				pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.1f, 1.0f, 10.0f), pGridBrush.Get());
				pCompatibleRenderTarget->EndDraw();

				// Retrieve the bitmap from the render target.
				Microsoft::WRL::ComPtr<ID2D1Bitmap> pGridBitmap;
				hr = pCompatibleRenderTarget->GetBitmap(pGridBitmap.ReleaseAndGetAddressOf());
				WOtech::ThrowIfFailed(hr);

				if (SUCCEEDED(hr))
				{
					// Create the bitmap brush.
					hr = m_deviceContext->CreateBitmapBrush(pGridBitmap.Get(), &m_gridBrush);
					WOtech::ThrowIfFailed(hr);
				}
			}
		}

		m_gridColor = color;
	}

	void SpriteBatch::DrawTextBlock(_In_ WOtech::TextBlock* const& text)
	{
		if (text)
		{
			auto rect = D2D1_RECT_F{ text->getPosition().x, text->getPosition().y, text->getlayoutbox().width, text->getlayoutbox().height };
			setRotation(rect, text->getRotation());
			m_deviceContext->DrawTextLayout(D2D1::Point2F(text->getPosition().x, text->getPosition().y), const_cast<IDWriteTextLayout*>(text->getLayout()), const_cast<ID2D1SolidColorBrush*>(text->getBrush()), D2D1_DRAW_TEXT_OPTIONS_NONE);
		}
	}
	void SpriteBatch::DrawString(_In_ WOtech::Font* const& font, _In_ FLOAT const& fontSize, _In_ D2D1_RECT_F const& layoutbox, _In_ DWRITE_FONT_STYLE const& style, _In_ D2D1_COLOR_F const& color, _In_ FLOAT const& rotation, _In_ winrt::hstring const& text)
	{
		HRESULT hr;

		// Set Color
		m_fontBrush->SetColor(color);

		// Create text format
		IDWriteTextFormat* temp = nullptr;
		hr = m_dwFactory->CreateTextFormat(font->getFontname().data(), font->getColletion(), DWRITE_FONT_WEIGHT_NORMAL, style, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &temp);
		WOtech::ThrowIfFailed(hr);

		// Create the Destination Rect
		D2D1_RECT_F destRect = D2D1::RectF(layoutbox.left, layoutbox.top, layoutbox.left + layoutbox.right, layoutbox.top + layoutbox.bottom);

		// Set Rotation
		setRotation(layoutbox, rotation);

		// Draw the text
		m_deviceContext->DrawText(text.data(), text.size(), temp, destRect, m_fontBrush.Get());

		SafeRelease(&temp);
	}

	void SpriteBatch::DrawBitmap(_In_ WOtech::Bitmap* const& bitmap)
	{
		m_deviceContext->DrawBitmap(bitmap->getBitmap());
	}
	void SpriteBatch::DrawBitmap(_In_ WOtech::Bitmap* const& bitmap, _In_  D2D1_RECT_F const& srcRect, _In_ D2D1_RECT_F const& destRect, _In_ FLOAT const& opacity, _In_ FLOAT const& rotation)
	{
		// Create a Rect to hold Position and Size of the Bitmap
		auto rect = D2D1_RECT_F{ destRect.left, destRect.top, srcRect.right, srcRect.bottom };

		// Set Rotation
		setRotation(rect, rotation);
		m_deviceContext->DrawBitmap(bitmap->getBitmap(), destRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);//wrapBitmapInterpolationMode(sprite->getInterpolation())
	}

	void SpriteBatch::DrawSprite(_In_ WOtech::Sprite* const& sprite)
	{
		// Create a Rect to hold Position and Size of the Sprite
		auto rect = D2D1_RECT_F{ sprite->getDestinationRect().left, sprite->getDestinationRect().top, sprite->getSourceRect().right, sprite->getSourceRect().bottom };

		// Set Rotation
		setRotation(rect, sprite->getRotation());

		auto spriteREct = sprite->getDestinationRect();

		// Set Transform
		setTransformation(sprite->getFlipMode());

		// Draw the Sprite
		m_deviceContext->DrawBitmap(sprite->getBitmap(), sprite->getDestinationRect(), sprite->getOpacity(), sprite->getInterpolation(), sprite->getSourceRect());
	}
	void SpriteBatch::DrawSprite(_In_ WOtech::Sprite* const& sprite, _In_ D2D1_RECT_F const& srcRect, _In_ D2D1_RECT_F const& destRect, _In_ FLOAT const& opacity, _In_ FLOAT const& rotation, _In_ WOtech::SPRITE_FLIP_MODE const& flipmode)
	{
		auto rect = D2D1_RECT_F{ destRect.left, destRect.top, destRect.right, destRect.bottom };// - .x , -.y

		// Set Rotation
		setRotation(rect, rotation);

		// Set Transform
		setTransformation(flipmode);

		// Draw the Sprite
		m_deviceContext->DrawBitmap(sprite->getBitmap(), destRect, opacity, sprite->getInterpolation(), srcRect);
	}

	void SpriteBatch::DrawAnimatedSprite(_In_ WOtech::AnimatedSprite* const& animatedsprite, _In_ winrt::hstring const& name)
	{
		// Create a Rect to hold Position and Size of the Sprite
		auto rect = D2D1_RECT_F{ animatedsprite->getPosition().x, animatedsprite->getPosition().y, animatedsprite->getFrameSize(name).width, animatedsprite->getFrameSize(name).height };

		// Set Rotation
		setRotation(rect, animatedsprite->getRotation());

		// Create the Destination Rect
		D2D1_RECT_F destRect = D2D1::RectF(animatedsprite->getPosition().x, animatedsprite->getPosition().y, (animatedsprite->getPosition().x + animatedsprite->getFrameSize(name).width) * animatedsprite->getScale(), (animatedsprite->getPosition().y + animatedsprite->getFrameSize(name).height) * animatedsprite->getScale());

		// Draw the AnimatedSprite
		m_deviceContext->DrawBitmap(animatedsprite->getBitmap(), destRect, animatedsprite->getOpacity(), animatedsprite->getInterpolation(), animatedsprite->getFrame(name));
	}

	void SpriteBatch::DrawGrid(_In_ D2D1_RECT_F const& area, _In_ D2D1_COLOR_F const& color, _In_ FLOAT const& rotation)
	{
		// Recolor grid
		m_gridColor = color;
		CreateGrid(color);

		// Set Rotation
		setRotation({ area.left, area.top, area.left + area.right, area.top + area.bottom }, rotation);

		// Draw Grid
		m_deviceContext->FillRectangle(D2D1::RectF(area.left, area.top, area.left + area.right, area.top + area.bottom), m_gridBrush.Get());
	}
	void SpriteBatch::DrawCircle(_In_ WOtech::Circle const& circle)
	{
		// Set Transformation
		m_deviceContext->SetTransform(m_deviceDX11->get2DOrientation());

		// Set Outline Color
		m_outlineBrush->SetColor(circle.Color);

		// Draw Circle
		m_deviceContext->DrawEllipse(D2D1::Ellipse(circle.Position, circle.Radius, circle.Radius), m_outlineBrush.Get(), circle.Tickness);
	}
	void SpriteBatch::DrawCircleOutlined(_In_ WOtech::CircleOutlined const& circleOutlined)
	{
		// Set Transformation
		m_deviceContext->SetTransform(m_deviceDX11->get2DOrientation());

		// Set Circle Color
		m_circleBrush->SetColor(circleOutlined.Color);

		// Set Outline Color
		m_outlineBrush->SetColor(circleOutlined.Outlinecolor);

		// Draw Circle
		m_deviceContext->FillEllipse(D2D1::Ellipse(circleOutlined.Position, circleOutlined.Radius, circleOutlined.Radius), m_circleBrush.Get());
		m_deviceContext->DrawEllipse(D2D1::Ellipse(circleOutlined.Position, circleOutlined.Radius - circleOutlined.Tickness / 2, circleOutlined.Radius - circleOutlined.Tickness / 2), m_outlineBrush.Get(), circleOutlined.Tickness);
	}
	void SpriteBatch::DrawCircleFilled(_In_ WOtech::CircleFilled const& circleFilled)
	{
		// Set Transformation
		m_deviceContext->SetTransform(m_deviceDX11->get2DOrientation());

		// Set Circle Color
		m_circleBrush->SetColor(circleFilled.Color);

		// Draw Circle
		m_deviceContext->FillEllipse(D2D1::Ellipse(circleFilled.Position, circleFilled.Radius, circleFilled.Radius), m_circleBrush.Get());
	}

	void SpriteBatch::DrawRectangle(_In_ WOtech::Rectangle const& rectangle)
	{
		// Set Rotation
		setRotation(rectangle.Area, rectangle.Rotation);

		// Set outline Color
		m_outlineBrush->SetColor(rectangle.Color);

		// Draw the Rectangle
		m_deviceContext->DrawRectangle(D2D1::RectF(rectangle.Area.left, rectangle.Area.top, rectangle.Area.left + rectangle.Area.right, rectangle.Area.top + rectangle.Area.bottom), m_outlineBrush.Get(), rectangle.Tickness);
	}
	void SpriteBatch::DrawRectangleOutlined(_In_ WOtech::RectangleOutlined const& rectangleOutlined)
	{
		// Set Rotation
		setRotation(rectangleOutlined.Area, rectangleOutlined.Rotation);

		// Set Rectangle Color
		m_rectangleBrush->SetColor(rectangleOutlined.Color);

		// Set Outline Color
		m_outlineBrush->SetColor(rectangleOutlined.Outlinecolor);

		// Draw the Rectangle
		m_deviceContext->FillRectangle(D2D1::RectF(rectangleOutlined.Area.left, rectangleOutlined.Area.top, rectangleOutlined.Area.left + rectangleOutlined.Area.right, rectangleOutlined.Area.top + rectangleOutlined.Area.bottom), m_rectangleBrush.Get());
		m_deviceContext->DrawRectangle(D2D1::RectF(rectangleOutlined.Area.left + rectangleOutlined.Tickness / 2, rectangleOutlined.Area.top + rectangleOutlined.Tickness / 2, (rectangleOutlined.Area.left + rectangleOutlined.Area.right) - rectangleOutlined.Tickness / 2, (rectangleOutlined.Area.top + rectangleOutlined.Area.bottom) - rectangleOutlined.Tickness / 2), m_outlineBrush.Get(), rectangleOutlined.Tickness);
	}
	void SpriteBatch::DrawRectangleFilled(_In_ WOtech::RectangleFilled const& rectangleFilled)
	{
		// Set Rotation
		setRotation(rectangleFilled.Area, rectangleFilled.Rotation);

		// Set Rectangle Color
		m_rectangleBrush->SetColor(rectangleFilled.Color);

		// Draw the Rectangle
		m_deviceContext->FillRectangle(D2D1::RectF(rectangleFilled.Area.left, rectangleFilled.Area.top, rectangleFilled.Area.left + rectangleFilled.Area.right, rectangleFilled.Area.top + rectangleFilled.Area.bottom), m_rectangleBrush.Get());
	}

	void SpriteBatch::DrawGeometry(_In_ WOtech::Geometry* const& geometry, _In_ FLOAT const& strokeWidth)
	{
		m_deviceContext->DrawGeometry(geometry->getGeometry(), geometry->getBrush(), strokeWidth);
	}

	ID2D1DeviceContext5* SpriteBatch::GetDeviceContext()
	{
		return m_deviceContext.Get();
	}

	ID2D1Factory6* SpriteBatch::getFactory()
	{
		return m_factory.Get();
	}

	WOtech::Image* SpriteBatch::getRenderTarget()
	{
		ID2D1Image* rendertarget = nullptr;

		m_deviceContext->GetTarget(&rendertarget);

		if (!rendertarget)
			throw winrt::hresult_invalid_argument{ L"No render target set!" };

		return new WOtech::Image(rendertarget);
	}

	D2D1_SIZE_F SpriteBatch::getLogicalSize()
	{
		return m_deviceDX11->getLogicalSize();
	}

	D2D1_RECT_F SpriteBatch::getRenderRect()
	{
		auto size = m_deviceContext->GetSize();

		return { 0.0f, 0.0f, size.width, size.height };
	}

	void SpriteBatch::setRenderTarget(_In_ WOtech::Image* const& rendertarget)
	{
		m_deviceContext->SetTarget(rendertarget->getImage());
	}
	void SpriteBatch::setRenderTarget(_In_ WOtech::Bitmap* const& rendertarget)
	{
		m_deviceContext->SetTarget(rendertarget->getBitmap());
	}

	void SpriteBatch::setRotation(_In_ D2D1_RECT_F const& area, _In_ FLOAT const& rotation)
	{
		FLOAT halfheight = area.bottom / 2;
		FLOAT halfwidth = area.right / 2;
		D2D1_SIZE_F logicalSize = m_deviceDX11->getLogicalSize();

		D2D1_POINT_2F  spriteCenter = D2D1::Point2F(logicalSize.width - (area.left + halfwidth), logicalSize.height - (area.top + halfheight));

		D2D1_MATRIX_3X2_F rotationMatrix = D2D1::Matrix3x2F::Rotation(rotation, spriteCenter);

		m_deviceContext->SetTransform(rotationMatrix * m_deviceDX11->get2DOrientation());
	}
	void SpriteBatch::setTransformation(_In_ WOtech::SPRITE_FLIP_MODE const& flipMode)
	{
		D2D1_MATRIX_3X2_F transformMatrix = D2D1::IdentityMatrix();

		switch (flipMode)
		{
		case WOtech::SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_NONE:
			// do nothing
			break;
		case WOtech::SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_HORIZONTAL:
			// todo: implement
			break;
		case WOtech::SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_VERTICAL:
			// todo: implement
			break;
		case WOtech::SPRITE_FLIP_MODE::SPRITE_FLIP_MODE_BOTH:
			// todo: implement
			break;
		default:
			break;
		}

		m_deviceContext->SetTransform(transformMatrix * m_deviceDX11->get2DOrientation());
	}

	void SpriteBatch::SortBatch()
	{
		switch (m_sortMode)
		{
		case WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_IMMEDIATE:
			return;// do nothing because everything is allready drawn
			break;
		case WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_DEFERRED:
			// todo: implement
			break;
		case WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_TEXTURE:
			// todo: implement
			break;
		case WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_BACKTOFRONT:
			// todo: implement
			break;
		case WOtech::SPRITE_SORT_MODE::SPRITE_SORT_MODE_FRONTTOBACK:
			// todo: implement
			break;
		default:
			return;// this should never happen -.-
			break;
		}
	}
	void SpriteBatch::DrawBatch()
	{
		return; // todo: draw Batch
	}
	void SpriteBatch::Release()
	{
		m_dwFactory.Reset();
		m_factory.Reset();
		m_deviceContext.Reset();
		m_device.Reset();
	}
}//namespace WOtech