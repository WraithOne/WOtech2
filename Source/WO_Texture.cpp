////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Texture.cpp
///
///			Description:
///
///			Created:	30.08.2014
///			Edited:		25.09.2018
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_DeviceDX11.hpp"
#include "WO_3DComponents.hpp"
#include "WO_Utilities.hpp"
#include "WICTextureLoader.hpp"

namespace WOtech
{
	Texture::Texture(_In_ winrt::hstring const& filename)
	{
		m_filenName = filename;
	}

	bool Texture::Load(_In_ DeviceDX11* const& device)
	{
		HRESULT hr;

		hr = DirectX::CreateWICTextureFromFile(device->getDevice(), device->getContext(), m_filenName.data(), nullptr, m_texture.ReleaseAndGetAddressOf(), 2048);

		ThrowIfFailed(hr);
		m_texture->GetDesc(&m_texDESC);

		return true;
	}

	void Texture::SubmitTexture(_In_ DeviceDX11* const& device, _In_ UINT const& slot)
	{
		auto context = device->getContext();

		context->PSSetShaderResources(slot, 1, m_texture.GetAddressOf());
	}

	ID3D11ShaderResourceView* Texture::getTexture()
	{
		return m_texture.Get();
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC Texture::getDescription()
	{
		return m_texDESC;
	}

	winrt::hstring Texture::getFilename()
	{
		return m_filenName;
	}
}//namespace WOtech