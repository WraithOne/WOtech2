////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: DefaultFactory.h
///
///			Description:
///
///			Created:	27.02.2016
///			Edited:		01.06.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_DEFAULTFACTORY_H
#define WO_DEFAULTFACTORY_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace WOtech
{
	///////////////////////
	// Forward declarations
	///////////////////////
	class DeviceDX11;
	interface IMaterial;
	class BasicMaterial;
	class Mesh;

	class DefaultFactory
	{
	public:
		static BasicMaterial* CreateBasicMaterial(_In_ WOtech::DeviceDX11* const& device);
		static Mesh* CreateCube(_In_ FLOAT const& size, _In_ WOtech::IMaterial* const& material, _In_ WOtech::DeviceDX11* const& device);
		static Mesh* CreateTriangle(_In_ FLOAT const& size, _In_ WOtech::IMaterial* const& material, _In_ WOtech::DeviceDX11* const& device);
	};
}
#endif