////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: 2DComponents.h
///
///			Description:
///
///			Created:	06.04.2016
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_2DCOMPONENTS_H
#define WO_2DCOMPONENTS_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	// Forward Declaration
	class SpriteBatch;

	enum SPRITE_FLIP_MODE
	{
		SPRITE_FLIP_MODE_NONE,
		SPRITE_FLIP_MODE_HORIZONTAL,
		SPRITE_FLIP_MODE_VERTICAL,
		SPRITE_FLIP_MODE_BOTH
	};

	struct Circle
	{
		D2D1_POINT_2F	Position;
		FLOAT			Radius;
		D2D1_COLOR_F	Color;
		FLOAT			Tickness;
		FLOAT			Rotation;
	};

	struct CircleOutlined
	{
		D2D1_POINT_2F	Position;
		FLOAT			Radius;
		D2D1_COLOR_F	Color;
		FLOAT			Tickness;
		D2D1_COLOR_F	Outlinecolor;
		FLOAT			Rotation;
	};

	struct CircleFilled
	{
		D2D1_POINT_2F	Position;
		FLOAT			Radius;
		D2D1_COLOR_F	Color;
		FLOAT			Rotation;
	};

	struct Rectangle
	{
		D2D1_RECT_F		Area;
		D2D1_COLOR_F	Color;
		FLOAT			Tickness;
		FLOAT			Rotation;
	};

	struct RectangleOutlined
	{
		D2D1_RECT_F		Area;
		D2D1_COLOR_F	Color;
		FLOAT			Tickness;
		D2D1_COLOR_F	Outlinecolor;
		FLOAT			Rotation;
	};

	struct RectangleFilled
	{
		D2D1_RECT_F		Area;
		D2D1_COLOR_F	Color;
		FLOAT			Rotation;
	};

	struct Animation
	{
		winrt::hstring	Name;
		UINT			Framecount;
		FLOAT			Frametime;
		FLOAT			Lastime;
		UINT			ActualFrame;
		D2D1_SIZE_F		FrameSize;
		D2D1_POINT_2F	SourcePosition;
	};

	class Image
	{
	public:
		Image();
		Image(_In_ ID2D1Image* const& image);

		~Image();

		void Reset();

		void setImage(_In_ ID2D1Image* const& image);
		ID2D1Image* getImage();

	private:
		Microsoft::WRL::ComPtr<ID2D1Image>	m_image;
	};

	class Bitmap
	{
	public:
		Bitmap();
		Bitmap(_In_ WOtech::SpriteBatch* const& spriteBatch, _In_ UINT const& width, _In_ UINT const& height);
		Bitmap(_In_ WOtech::SpriteBatch* const& spriteBatch, _In_ winrt::hstring const& fileName);
		Bitmap(_In_ ID2D1Bitmap1* const& bitmap);

		~Bitmap();

		void Reset();

		const D2D1_SIZE_F getSize();
		const D2D1_RECT_F getSourceRECT();

		void setBitmap(_In_ ID2D1Bitmap1* const& bitmap);
		ID2D1Bitmap1* getBitmap();

	private:
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>	m_bitmap;
	};

	class Sprite
	{
	public:
		Sprite(_In_ winrt::hstring const& filename);

		~Sprite();

		void Load(_In_ WOtech::SpriteBatch* const& spriteBatch);
		void UnLoad();

		//Setter
		void setPosition(_In_ FLOAT const& x, _In_ FLOAT const& y);
		void setSize(_In_ FLOAT const& width, _In_ FLOAT const& height);
		void setSourceRect(_In_ FLOAT const& x, _In_ FLOAT const& y, _In_ FLOAT const& height, _In_ FLOAT const& width);
		void setOpacity(_In_ FLOAT const& opacity);
		void setRotation(_In_ FLOAT const& degree);
		void setRotationinRadian(_In_ FLOAT const& radian);
		void setFlipMode(_In_ WOtech::SPRITE_FLIP_MODE const& flipmode);
		void setInterpolation(_In_ D2D1_INTERPOLATION_MODE const& interpolation);

		//Getter
		ID2D1Bitmap1* getBitmap();

		D2D1_RECT_F							getSourceRect();
		D2D1_RECT_F							getDestinationRect();

		D2D1_POINT_2F						getPosition();
		D2D1_SIZE_F							getSize();
		FLOAT								getOpacity();
		FLOAT								getRotation();
		WOtech::SPRITE_FLIP_MODE			getFlipMode();
		D2D1_INTERPOLATION_MODE				getInterpolation();

	private:
		winrt::hstring							m_fileName;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>	m_bitmap;

		D2D1_RECT_F								m_sourceRect;
		D2D1_POINT_2F							m_position;
		D2D1_SIZE_F								m_size;
		FLOAT									m_rotation;
		FLOAT									m_opacity;
		WOtech::SPRITE_FLIP_MODE				m_flipMode;
		D2D1_INTERPOLATION_MODE					m_interpolation;
	};// class Sprite

	class AnimatedSprite
	{
	public:
		AnimatedSprite(_In_ winrt::hstring const& filename);
		AnimatedSprite(_In_ WOtech::Bitmap* const& bitmap);

		~AnimatedSprite();

		void Load(_In_ WOtech::SpriteBatch* const& spriteBatch);
		void UnLoad();

		bool AddAnimation(_In_ winrt::hstring const& name, _In_ UINT const& framecount, _In_ FLOAT const& frametime, _In_ D2D1_SIZE_F const& framesize, _In_ D2D1_POINT_2F const& sourceposition);
		void Update(_In_ winrt::hstring const& name, _In_ FLOAT const& elapsed);
		void Restart(_In_ winrt::hstring const& name);

		//Setter
		ID2D1Bitmap* getBitmap();

		void setPosition(_In_ FLOAT const& x, _In_ FLOAT const& y);
		void setScale(_In_ FLOAT const& scale);
		void setOpacity(_In_ FLOAT const& opacity);
		void setRotation(_In_ FLOAT const& degree);
		void setRotationinRadian(_In_ FLOAT const& radian);
		void setFlipMode(_In_ SPRITE_FLIP_MODE const& flipmode);
		void setInterpolation(_In_ D2D1_INTERPOLATION_MODE const& interpolation);

		//Getter
		D2D1_POINT_2F				getPosition();
		FLOAT						getScale();
		D2D1_SIZE_F					getFrameSize(_In_ winrt::hstring const& name);
		D2D1_RECT_F					getFrame(_In_ winrt::hstring const& name);
		FLOAT						getOpacity();
		FLOAT						getRotation();
		WOtech::SPRITE_FLIP_MODE	getFlipMode();
		D2D1_INTERPOLATION_MODE		getInterpolation();

	private:
		bool								m_fromFile;
		winrt::hstring						m_fileName;

		Microsoft::WRL::ComPtr<ID2D1Bitmap>	m_bitmap;

		std::list<WOtech::Animation>		m_animationList;

		D2D1_POINT_2F						m_position;
		FLOAT								m_scale;
		FLOAT								m_rotation;
		WOtech::SPRITE_FLIP_MODE			m_flipMode;
		FLOAT								m_opacity;
		D2D1_INTERPOLATION_MODE				m_interpolation;
	};

	class Font
	{
	public:
		Font(_In_ winrt::hstring const& filename);

		~Font();

		void Load();
		void UnLoad();

		// GETTER
		winrt::hstring			getFontname();

		IDWriteFontCollection* getColletion();
		IDWriteFontCollection** getCollectionL();

	private:
		winrt::hstring									m_fileName;
		Microsoft::WRL::ComPtr<IDWriteFontCollection>	m_collection;
	};//class Font

	class TextBlock
	{
	public:
		void CreateText(_In_ WOtech::Font* const& font, _In_  WOtech::SpriteBatch* const& spriteBatch);
		void CreateText(_In_ WOtech::Font* const& font,
			_In_  WOtech::SpriteBatch* const& spriteBatch,
			_In_ FLOAT const& size,
			_In_ winrt::hstring const& text,
			_In_ DWRITE_FONT_STYLE const& style,
			_In_ D2D1_COLOR_F const& color,
			_In_ D2D1_POINT_2F const& position,
			_In_ D2D1_SIZE_F const& layoutsize);

		~TextBlock();

		// Setters
		void setSize(_In_ FLOAT const& size);

		void setText(_In_ winrt::hstring const& text);

		void setStyle(_In_ DWRITE_FONT_STYLE const& style);

		void setPosition(_In_ FLOAT const& x, _In_ FLOAT const& y);
		void setPosition(_In_ D2D1_POINT_2F const& position);

		void setRotation(_In_ FLOAT const& degree);
		void setRotationinRadian(_In_ FLOAT const& radian);
		void setRotationinVector(_In_ FLOAT const& x, _In_ FLOAT const& y);

		void setLayoutBox(_In_ FLOAT const& w, _In_ FLOAT const& h);
		void setLayoutBox(_In_ D2D1_SIZE_F const& layoutboxsize);

		void setColor(_In_ FLOAT const& r, _In_ FLOAT const& g, _In_ FLOAT const& b, _In_ FLOAT const& a);
		void setColor(_In_ D2D1_COLOR_F const& color);

		// Getters
		D2D1_POINT_2F			getPosition();
		FLOAT					getRotation();
		D2D1_SIZE_F				getlayoutbox();

		IDWriteTextLayout*		getLayout();
		ID2D1SolidColorBrush*	getBrush();

	private:
		void makeText();
		void makeBrush(_In_ WOtech::SpriteBatch* const& spriteBatch);

	private:
		Microsoft::WRL::ComPtr<IDWriteFactory>			m_wFactory;

		Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_format;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>		m_layout;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_brush;
		WOtech::Font*									m_font;

		FLOAT											m_size;
		winrt::hstring									m_text;
		DWRITE_FONT_STYLE								m_style;
		D2D1_POINT_2F									m_position;
		FLOAT											m_rotation;
		D2D1_SIZE_F										m_layoutbox;

		bool											m_changed;
	};//class Text

	class Geometry
	{
	public:
		Geometry();

		~Geometry();

		void Create(_In_ WOtech::SpriteBatch* const& spritebatch);

		void OpenGeometry();

		void StartFigure(_In_ D2D1_POINT_2F const& strartposition, _In_ D2D1_FIGURE_BEGIN const& begin);
		void addArc(_In_ D2D1_ARC_SEGMENT const& arcsegment);
		void addBezier(_In_ D2D1_BEZIER_SEGMENT const& beziersegment);
		void addLine(_In_ D2D1_POINT_2F const& lineend);
		void EndFigure(_In_ D2D1_FIGURE_END const& figureend);

		void CloseGeometry();

		void setColor(_In_ D2D1_COLOR_F const& color);

		ID2D1PathGeometry*		getGeometry();
		ID2D1SolidColorBrush*	getBrush();

	private:
		Microsoft::WRL::ComPtr<ID2D1PathGeometry>		m_geometryPath;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink>		m_geometrySink;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_geometryBrush;
	};//class Geometry
}
#endif
