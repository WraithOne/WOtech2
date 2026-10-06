////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: DeviceDX11.h
///
///			Description:
///
///			Created:	31.03.2016
///			Edited:		07.01.2018
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_DEVICEDX11_H
#define WO_DEVICEDX11_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

namespace ScreenRotation
{
	// 0-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 90-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 180-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 270-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation270(
		0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
};// namespace ScreenRotation

namespace WOtech
{
	// Forward Declaration
	class Window;

	class DeviceDX11
	{
	public:
		DeviceDX11() = delete;
		DeviceDX11(_In_ WOtech::Window* const& window);

		~DeviceDX11();

		void Create();

		void Clear(_In_ D2D1_COLOR_F const& color);
		void Clear(_In_ D2D1_COLOR_F const& color, _In_ D3D11_CLEAR_FLAG const& clearFlags, _In_ FLOAT const& depth, _In_ UINT8 const& stencil);

		void Present();
		void ValidateDevice();
		void HandleDeviceLost();
		void Trim();

		void setLogicalSize(_In_ D2D1_SIZE_F const& logicalSize);
		void setCurrentOrientation(_In_ winrt::Windows::Graphics::Display::DisplayOrientations const& currentOrientation);
		void setDpi(_In_ FLOAT dpi);
		void setCompositionScale(_In_ FLOAT const& compositionScaleX, _In_ FLOAT const& compositionScaleY);

		void setSampling(_In_ UINT const& count, _In_ UINT const& quality);
		void setDepthStencil(_In_ bool const& enable);
		void setWireframe(_In_ bool const& enable);
		void setViewPort(_In_ FLOAT const& topleftx, _In_ FLOAT const& toplefty, _In_ FLOAT const& width, _In_ FLOAT const& height, _In_ FLOAT const& mindept, _In_ FLOAT const& maxdept, _In_ bool const& useActualOriantation);
		void setStereoSwapChain(_In_ bool const& enable);

		void PIXBeginEvent(_In_ winrt::hstring const& name);
		void PIXEndEvent();
		void PIXSetMarker(_In_ winrt::hstring const& name);

		void EnumerateAdapters(_Out_ std::list<IDXGIAdapter4*>* adapterList);
		void EnumerateOutputs(_In_ IDXGIAdapter4* const& adapter, _Out_ std::list<IDXGIOutput6*>* outputList);
		void EnumerateDisplayModes(_In_ IDXGIOutput6* const& output, _Out_ std::list<DXGI_MODE_DESC1*>* displayModeList);

		void setRenderTarget(_In_ ID3D11RenderTargetView* const& target);

		void setPresentationParams(_In_ DXGI_PRESENT_PARAMETERS const& params);

		IDXGIFactory5*				getFactory();
		ID3D11Device5*				getDevice();
		IDXGIDevice4*				getDXGIDevice();
		ID3D11DeviceContext4*		getContext();

		ID3D11RenderTargetView1*	getRenderTarget();
		IDXGISurface2*				getSurface();

		ID3D11DepthStencilState*	getDepthStencilState();
		ID3D11RasterizerState2*		getRasterizerState();
		D3D11_VIEWPORT				getViewPort();

		D2D1::Matrix3x2F			get2DOrientation();
		DirectX::XMFLOAT4X4			get3DOrientation();

		DXGI_PRESENT_PARAMETERS		getPresentationParams();

		UINT						getSampleCount();
		UINT						getSampleQuality();

		FLOAT						getDPI();

		D2D1_SIZE_F					getLogicalSize();
		D2D1_SIZE_F					getRenderTargetSize();

		bool						isStereoSwapChain();
		
	private:
		void						CreateDevices();
		void						CreateWindowSizeDependentResources();

		void						RecreateSwapChain();

		DXGI_MODE_ROTATION			ComputeDisplayRotation();

	private:
		WOtech::Window*											m_window;

		Microsoft::WRL::ComPtr<ID3D11Device5>					m_device;
		Microsoft::WRL::ComPtr<IDXGIDevice4>					m_dxgiDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4>			m_context;
		Microsoft::WRL::ComPtr<IDXGIFactory5>					m_factory;

		bool													m_recreateSwapChain;
		Microsoft::WRL::ComPtr<IDXGISwapChain4>					m_swapChain;
		DXGI_FORMAT												m_swapChainFormat;
		Microsoft::WRL::ComPtr<IDXGISurface2>					m_dxgiBackBuffer;
		UINT													m_backBufferCount;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>			m_renderTargetView;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>			m_depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D1>				m_depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			m_depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState2>			m_rasterizerState;

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>		m_userAnnotation;

		D3D11_VIEWPORT											m_viewport;
		D3D_FEATURE_LEVEL										m_featureLevel;

		D3D_DRIVER_TYPE											m_driverType;
		DXGI_ADAPTER_DESC										m_currentAdapterDescription;

		winrt::Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
		winrt::Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;

		DirectX::XMFLOAT4X4										m_orientationTransform3D;
		D2D1::Matrix3x2F										m_orientationTransform2D;

		FLOAT													m_compositionScaleX;
		FLOAT													m_compositionScaleY;
		D2D1_SIZE_F												m_logicalSize;
		FLOAT													m_dpi;

		D2D1_SIZE_F												m_d3dRenderTargetSize;
		D2D1_SIZE_F												m_outputSize;

		DXGI_PRESENT_PARAMETERS									m_presentParameters;
		UINT													m_sampleQuality;
		UINT													m_sampleCount;

		bool													m_stereo;
	};// class DeviceDX11
}// namespace WOtech
#endif