////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Geometry.cpp
///
///			Description:
///
///			Created:	14.11.2016
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
	Geometry::Geometry()
	{
		// todo: Content Manager
	}
	void WOtech::Geometry::Create(_In_ WOtech::SpriteBatch* const& spritebatch)
	{
		HRESULT hr;
		// Create GeometryPath
		hr = spritebatch->getFactory()->CreatePathGeometry(m_geometryPath.ReleaseAndGetAddressOf());
		ThrowIfFailed(hr);
		// Create Geometrybrush
		hr = spritebatch->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), m_geometryBrush.ReleaseAndGetAddressOf());
		ThrowIfFailed(hr);
	}

	void Geometry::OpenGeometry()
	{
		HRESULT hr;
		hr = m_geometryPath->Open(m_geometrySink.ReleaseAndGetAddressOf());
	}

	void Geometry::StartFigure(_In_ D2D1_POINT_2F const& startposition, _In_ D2D1_FIGURE_BEGIN const& begin)
	{
		m_geometrySink->BeginFigure(startposition, begin);
	}
	void Geometry::addArc(_In_ D2D1_ARC_SEGMENT const& arcsegment)
	{
		m_geometrySink->AddArc(arcsegment);
	}
	void Geometry::addBezier(_In_ D2D1_BEZIER_SEGMENT const& beziersegment)
	{
		m_geometrySink->AddBezier(beziersegment);
	}
	void Geometry::addLine(_In_ D2D1_POINT_2F const& lineend)
	{
		m_geometrySink->AddLine(lineend);
	}
	void Geometry::EndFigure(_In_ D2D1_FIGURE_END const& figureend)
	{
		m_geometrySink->EndFigure(figureend);
	}

	void Geometry::CloseGeometry()
	{
		HRESULT hr;
		hr = m_geometrySink->Close();
		ThrowIfFailed(hr);
	}

	void Geometry::setColor(_In_ D2D1_COLOR_F const& color)
	{
		m_geometryBrush->SetColor(color);
	}

	ID2D1PathGeometry* Geometry::getGeometry()
	{
		return m_geometryPath.Get();
	}
	ID2D1SolidColorBrush* Geometry::getBrush()
	{
		return m_geometryBrush.Get();
	}
	Geometry::~Geometry()
	{
		// todo: Content Manager
	}
}