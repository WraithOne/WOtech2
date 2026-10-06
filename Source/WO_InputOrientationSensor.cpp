////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: InputOrientationSensor.cpp
///
///			Description:
///
///			Created:	26.08.2016
///			Edited:		01.06.2017
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Input.hpp"

namespace WOtech
{
	bool InputManager::OrientationSensorConnected()
	{
		auto sensor = winrt::Windows::Devices::Sensors::OrientationSensor::GetDefault();

		if (sensor != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void InputManager::ActivateOrientationSensor(_In_ winrt::Windows::Devices::Sensors::SensorReadingType const& Type, _In_ winrt::Windows::Devices::Sensors::SensorOptimizationGoal const& Goal, _In_ UINT const& Interval)
	{
		m_orientationSensor = winrt::Windows::Devices::Sensors::OrientationSensor::GetDefault(Type, Goal);

		if (m_orientationSensor != nullptr)
		{
			m_orientationSensor.ReportInterval((std::max)(m_orientationSensor.MinimumReportInterval(), Interval));
			m_orientationToken = m_orientationSensor.ReadingChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Devices::Sensors::OrientationSensor, winrt::Windows::Devices::Sensors::OrientationSensorReadingChangedEventArgs>(this, &WOtech::InputManager::ReadingChanged));
			m_orientationActive = true;
		}
		else
		{
			throw winrt::hresult_invalid_argument{ L"No OrientationSensor pressend" };
		}
	}
	void InputManager::DeactivateOrientationSensor()
	{
		m_orientationSensor.ReportInterval(0U);
		m_orientationSensor.ReadingChanged(nullptr);// -= m_orientationToken
		m_orientationActive = false;
	}
	bool InputManager::OrientationSensorActive()
	{
		return m_orientationActive;
	}
	winrt::Windows::Devices::Sensors::SensorQuaternion InputManager::OrientationSensorQuaternion(void)
	{
		return m_orientationSensorReading.Quaternion();
	}
	winrt::Windows::Devices::Sensors::SensorRotationMatrix InputManager::OrientationSensorMatrix(void)
	{
		return m_orientationSensorReading.RotationMatrix();
	}
}// namespace WOtech