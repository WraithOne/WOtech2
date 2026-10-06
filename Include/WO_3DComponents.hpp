////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: 3DComponents.h
///
///			Description:
///
///			Created:	22.02.2016
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_3DCOMPONENTS_H
#define WO_3DCOMPONENTS_H

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

	class Camera
	{
	public:
		Camera();

		void SetViewParams(_In_ DirectX::XMFLOAT3 const& eye, _In_ DirectX::XMFLOAT3 const& lookAt, _In_ DirectX::XMFLOAT3 const& up);
		void SetProjParams(_In_ FLOAT const& fieldOfView, _In_ FLOAT const& aspectRatio, _In_ FLOAT const& nearPlane, _In_ FLOAT const& farPlane);

		void LookDirection(_In_ DirectX::XMFLOAT3 const& lookDirection);
		void Eye(_In_ DirectX::XMFLOAT3 const& position);

		FLOAT	NearClipPlane();
		FLOAT	FarClipPlane();
		FLOAT	Pitch();
		FLOAT	Yaw();

		DirectX::XMFLOAT3 Eye();
		DirectX::XMFLOAT3 LookAt();
		DirectX::XMFLOAT3 Up();

		DirectX::XMMATRIX ViewMatrix();
		DirectX::XMMATRIX ProjectionMatrix();
		DirectX::XMMATRIX InverseMatrix();

	private:
		DirectX::XMFLOAT4X4	m_viewMatrix;
		DirectX::XMFLOAT4X4 m_projectionMatrix;
		DirectX::XMFLOAT4X4 m_inverseView;

		DirectX::XMFLOAT3	m_eye;
		DirectX::XMFLOAT3	m_lookAt;
		DirectX::XMFLOAT3	m_up;

		FLOAT				m_cameraYawAngle;
		FLOAT				m_cameraPitchAngle;

		FLOAT				m_fieldOfView;
		FLOAT				m_aspectRatio;
		FLOAT				m_nearPlane;
		FLOAT				m_farPlane;
	};//class Camera

	class ComputeShader
	{
	public:
		ComputeShader();

		~ComputeShader();

	private:
	};//class ComputeShader

	class VertexShader
	{
	public:
		VertexShader() = delete;
		VertexShader(_In_ winrt::hstring const& compiledVertexShaderObject, _In_ WOtech::DeviceDX11* const& device);
		VertexShader(_In_ winrt::hstring const& compiledVertexShaderObject, _In_ D3D11_INPUT_ELEMENT_DESC* const& inputElementDesc, _In_ WOtech::DeviceDX11* const& device);
		VertexShader(_In_ winrt::hstring const& filename, _In_ winrt::hstring const& entryPoint, _In_ INT const& unUsed, _In_ WOtech::DeviceDX11* const& device);
		VertexShader(_In_ winrt::hstring const& filename, _In_ winrt::hstring const& entryPoint, _In_ const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, _In_ INT const& unUsed, _In_ WOtech::DeviceDX11* const& device);
		VertexShader(_In_ void* const& pShaderBytecode, _In_ size_t const& BytecodeLength, _In_opt_ const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, _In_ INT const& unUsed, _In_ INT const& unUsed2, _In_ WOtech::DeviceDX11* const& device);

		void Load(_In_ WOtech::DeviceDX11* const& device);

		//Getter
		ID3D11VertexShader* getShader();
		ID3D11InputLayout* getInputLayout();

	private:
		void ComilefromFile(_In_ WOtech::DeviceDX11* const& device, _In_ winrt::hstring const& filename, _In_ winrt::hstring const& entryPoint);
		void LoadfromFile(_In_ WOtech::DeviceDX11* const& device, _In_ winrt::hstring const& compiledVertexShaderObject);
		void LoadfromByteArray(_In_ WOtech::DeviceDX11* const& device, _In_ void const* const& pShaderBytecode, _In_ size_t const& BytecodeLength);

		void CreateInputLayout(_In_ WOtech::DeviceDX11* const& device);
		void ReflectInputLayout(_In_ WOtech::DeviceDX11* const& device);

	private:
		bool										m_loadfromFile;
		winrt::hstring								m_fileName;
		winrt::hstring								m_entryPoint;

		bool										m_useCVSO;
		winrt::hstring								m_CVSO;

		bool										m_useShaderByteCode;
		void* m_shaderByteCode;
		SIZE_T										m_byteCodeLength;

		bool										m_useInputElementDesc;
		const D3D11_INPUT_ELEMENT_DESC*				m_inputElementDesc;

		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob>			m_vertexBlob;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
	};//class VertexShader

	class PixelShader
	{
	public:
		PixelShader() = delete;
		PixelShader(_In_ winrt::hstring const& CSOFilename, _In_ WOtech::DeviceDX11* const& device);
		PixelShader(_In_ void* const& ShaderBytecode, _In_ size_t const& BytecodeLength, _In_ WOtech::DeviceDX11* const& device);

		void Load(_In_ WOtech::DeviceDX11* const& device);

		//Getter
		ID3D11PixelShader* getShader();

	private:
		void LoadfromFile(_In_ WOtech::DeviceDX11* const& device);
		void LoadfromByteArray(_In_ WOtech::DeviceDX11* const& device);

	private:
		bool										m_useBytecode;

		winrt::hstring								m_csoFilename;

		void* m_shaderByteCode;
		SIZE_T										m_BytecodeLength;

		Microsoft::WRL::ComPtr<ID3DBlob>			m_pixelBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	};//class Pixelshader

	class Texture
	{
	public:
		Texture() = delete;
		Texture(_In_ winrt::hstring const& filename);

		bool Load(_In_ WOtech::DeviceDX11* const& device);
		void SubmitTexture(_In_ WOtech::DeviceDX11* const& device, _In_ UINT const& slot);

		// Getter
		winrt::hstring					getFilename();

		ID3D11ShaderResourceView*		getTexture();
		D3D11_SHADER_RESOURCE_VIEW_DESC	getDescription();

	private:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

	private:
		winrt::hstring										m_filenName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
		D3D11_SHADER_RESOURCE_VIEW_DESC						m_texDESC;
	};// class Texture

	class VertexBuffer
	{
	public:
		VertexBuffer() = delete;
		VertexBuffer(_In_ void* const& data, _In_  UINT const& size, _In_  UINT const& stride, _In_ UINT const& offset, _In_  WOtech::DeviceDX11* const& device);

		void SubmitBuffer(_In_ WOtech::DeviceDX11* const& device);

		void setOffset(_In_ UINT const& offset);
		void setStride(_In_ UINT const& stride);

		UINT32 getStride();
		UINT32 getOffset();

		void CreateBuffer(_In_ void* const& data, _In_  UINT const& size, _In_  WOtech::DeviceDX11* const& device);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
		UINT32									m_stride;
		UINT32									m_offset;
	};//class VertexBuffer

	class IndexBuffer
	{
	public:
		IndexBuffer() = delete;
		IndexBuffer(_In_ void* const& data, _In_  UINT const& count, _In_  WOtech::DeviceDX11* const& device);

		void SubmitBuffer(_In_ WOtech::DeviceDX11* const& device);
		inline UINT getCount() { return m_count; }

		void CreateBuffer(_In_ void* const& data, _In_  UINT const& count, _In_  WOtech::DeviceDX11* const& device);

	private:
		UINT32									m_count;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;
	};//class IndexBuffer

	class ConstantBuffer
	{
	public:
		ConstantBuffer();

		~ConstantBuffer();

	private:
	};//class ConstantBuffer

	class Mesh
	{
	public:
		Mesh() = delete;
		Mesh(_In_ WOtech::VertexBuffer* const& vertexBuffer, _In_ WOtech::IndexBuffer* const& indexBuffer, _In_ WOtech::IMaterial* const& material, _In_ D3D11_INPUT_ELEMENT_DESC* const& inputLayout);
		~Mesh();

		void setGeometry(_In_ WOtech::VertexBuffer* const& vertex, _In_ WOtech::IndexBuffer* const& index);
		void setMaterial(_In_ WOtech::IMaterial* const& material);
		void setInputLayout(_In_ D3D11_INPUT_ELEMENT_DESC* const& inputLayout);
		void setPosition(_In_  DirectX::XMFLOAT3 const& position);
		void setScaling(_In_ DirectX::XMFLOAT3 const& scaling);
		void setRotation(_In_ DirectX::XMFLOAT3 const& rotation);

		void bindMaterial(_In_ WOtech::DeviceDX11* device);
		void Render(_In_ WOtech::DeviceDX11* device);

		const DirectX::XMFLOAT3			getPosition();
		const DirectX::XMFLOAT3			getScaling();
		const DirectX::XMFLOAT3			getRotation();
		const DirectX::XMMATRIX			GetWorldMatrix();
		const WOtech::IMaterial*		GetMaterial();
		const D3D11_INPUT_ELEMENT_DESC* getInputLayout();

	private:
		WOtech::VertexBuffer*		m_vertexBuffer;
		WOtech::IndexBuffer*		m_indexBuffer;
		WOtech::IMaterial*			m_material;
		D3D11_INPUT_ELEMENT_DESC*	m_inputLayout;
		DirectX::XMFLOAT3			m_position;
		DirectX::XMFLOAT3			m_scaling;
		DirectX::XMFLOAT3			m_rotation;
	};//class Mesh
}// WOtech
#endif