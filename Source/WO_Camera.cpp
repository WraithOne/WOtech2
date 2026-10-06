////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Camera.cpp
///
///			Description:
///
///			Created:	10.05.2014
///			Edited:		27.11.2025
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include <WO_pch.hpp>
#include <WO_3DComponents.hpp>
#include <WO_Utilities.hpp>

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

namespace WOtech
{
	Camera::Camera()
	{
		// Setup the view matrix.
		DirectX::XMFLOAT3 eye;
		eye.x = 0.0f;
		eye.y = 0.0f;
		eye.z = 0.0f;

		DirectX::XMFLOAT3 lookAt;
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		DirectX::XMFLOAT3 up;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		SetViewParams(eye, lookAt, up);

		// Setup the projection matrix.
		// FOV 60
		// AspectRatio 16:9
		// NearPlane 0.01
		// Farplane 1000
		SetProjParams(60.0f, 1.7777f, 0.01f, 1000.0f);
	}

	void Camera::LookDirection(_In_ DirectX::XMFLOAT3 const& lookDirection)
	{
		DirectX::XMFLOAT3 lookAt;
		lookAt.x = m_eye.x + lookDirection.x;
		lookAt.y = m_eye.y + lookDirection.y;
		lookAt.z = m_eye.z + lookDirection.z;

		SetViewParams(m_eye, lookAt, m_up);
	}

	void Camera::Eye(_In_ DirectX::XMFLOAT3 const& eye)
	{
		SetViewParams(eye, m_lookAt, m_up);
	}

	void Camera::SetViewParams(_In_ DirectX::XMFLOAT3 const& eye, _In_ DirectX::XMFLOAT3 const& lookAt, _In_ DirectX::XMFLOAT3 const& up)
	{
		m_eye = eye;
		m_lookAt = lookAt;
		m_up = up;

		// Calculate the view matrix.
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtRH(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));

		DirectX::XMVECTOR det;
		DirectX::XMMATRIX inverseView = XMMatrixInverse(&det, view);
		XMStoreFloat4x4(&m_viewMatrix, view);
		XMStoreFloat4x4(&m_inverseView, inverseView);

		// The axis basis vectors and camera position are stored inside the
		// position matrix in the 4 rows of the camera's world matrix.
		// To figure out the yaw/pitch of the camera, we just need the Z basis vector.
		DirectX::XMFLOAT3 zBasis;
		DirectX::XMStoreFloat3(&zBasis, inverseView.r[2]);

		m_cameraYawAngle = atan2f(zBasis.x, zBasis.z);

		FLOAT len = sqrtf(zBasis.z * zBasis.z + zBasis.x * zBasis.x);
		m_cameraPitchAngle = atan2f(zBasis.y, len);
	}

	void Camera::SetProjParams(_In_ FLOAT const& fieldOfView, _In_ FLOAT const& aspectRatio, _In_ FLOAT const& nearPlane, _In_ FLOAT const& farPlane)
	{
		// Set attributes for the projection matrix.
		m_fieldOfView = DegreetoRadian(fieldOfView);
		m_aspectRatio = aspectRatio;
		if (aspectRatio < 1.0f)
		{
			// Portrait Oriantation
			m_fieldOfView *= 2.0f;// todo: add resolution/oriantation to arguments for proper scaling
		}
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixPerspectiveFovRH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane));
	}

	DirectX::XMMATRIX Camera::ViewMatrix()
	{
		return DirectX::XMLoadFloat4x4(&m_viewMatrix);
	}

	DirectX::XMMATRIX Camera::ProjectionMatrix()
	{
		return DirectX::XMLoadFloat4x4(&m_projectionMatrix);
	}

	DirectX::XMMATRIX Camera::InverseMatrix()
	{
		return DirectX::XMLoadFloat4x4(&m_inverseView);
	}

	DirectX::XMFLOAT3 Camera::Eye()
	{
		return m_eye;
	}

	DirectX::XMFLOAT3 Camera::LookAt()
	{
		return m_lookAt;
	}

	DirectX::XMFLOAT3 Camera::Up()
	{
		return m_up;
	}

	FLOAT Camera::NearClipPlane()
	{
		return m_nearPlane;
	}

	FLOAT Camera::FarClipPlane()
	{
		return m_farPlane;
	}

	FLOAT Camera::Pitch()
	{
		return m_cameraPitchAngle;
	}

	FLOAT Camera::Yaw()
	{
		return m_cameraYawAngle;
	}
}//namespace WOtech