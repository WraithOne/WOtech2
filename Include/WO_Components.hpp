////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Components.hpp
///
///			Description:
///			ECS component data. Graphics and audio objects are wrapped,
///			not duplicated. Physics handles stay opaque (no box3d types).
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_COMPONENTS_H
#define WO_COMPONENTS_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "WO_3DComponents.hpp"

#include <string>

namespace WOtech
{
	class Mesh;

	enum COMPONENT_TYPE
	{
		COMPONENT_TYPE_NONE = 0,
		COMPONENT_TYPE_TRANSFORM = 1,
		COMPONENT_TYPE_CAMERA = 2,
		COMPONENT_TYPE_MESH_RENDERER = 3,
		COMPONENT_TYPE_RIGID_BODY = 4,
		COMPONENT_TYPE_COLLIDER = 5,
		COMPONENT_TYPE_AUDIO_SOURCE = 6,
		COMPONENT_TYPE_SCRIPT = 7,
		COMPONENT_TYPE_LIGHT = 8,
		COMPONENT_TYPE_PREFAB = 9
	};

	enum BODY_TYPE
	{
		BODY_TYPE_STATIC = 0,
		BODY_TYPE_KINEMATIC = 1,
		BODY_TYPE_DYNAMIC = 2
	};

	enum COLLIDER_SHAPE
	{
		COLLIDER_SHAPE_BOX = 0,
		COLLIDER_SHAPE_SPHERE = 1,
		COLLIDER_SHAPE_CAPSULE = 2
	};

	enum LIGHT_KIND
	{
		LIGHT_KIND_DIRECTIONAL = 0,
		LIGHT_KIND_POINT = 1,
		LIGHT_KIND_SPOT = 2
	};

	struct Transform
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Rotation;
		DirectX::XMFLOAT3 Scale;

		Transform()
			: Position(0.0f, 0.0f, 0.0f)
			, Rotation(0.0f, 0.0f, 0.0f)
			, Scale(1.0f, 1.0f, 1.0f)
		{
		}
	};

	struct CameraComponent
	{
		WOtech::Camera View;
		FLOAT FieldOfViewDegrees;
		bool Primary;

		CameraComponent()
			: FieldOfViewDegrees(60.0f)
			, Primary(false)
		{
		}
	};

	struct MeshRenderer
	{
		WOtech::Mesh* MeshObject;
		std::string AssetPath;
		FLOAT BaseColor[4];
		FLOAT Metallic;
		FLOAT Roughness;
		bool CastShadow;
		bool ReceiveShadow;

		MeshRenderer()
			: MeshObject(nullptr)
			, Metallic(0.0f)
			, Roughness(0.5f)
			, CastShadow(true)
			, ReceiveShadow(true)
		{
			BaseColor[0] = 1.0f;
			BaseColor[1] = 1.0f;
			BaseColor[2] = 1.0f;
			BaseColor[3] = 1.0f;
		}
	};

	struct RigidBody
	{
		BODY_TYPE Type;
		FLOAT Mass;
		DirectX::XMFLOAT3 LinearVelocity;
		DirectX::XMFLOAT3 AngularVelocity;
		bool UseGravity;
		UINT64 PhysicsHandle;

		RigidBody()
			: Type(BODY_TYPE_DYNAMIC)
			, Mass(1.0f)
			, LinearVelocity(0.0f, 0.0f, 0.0f)
			, AngularVelocity(0.0f, 0.0f, 0.0f)
			, UseGravity(true)
			, PhysicsHandle(0)
		{
		}
	};

	struct Collider
	{
		COLLIDER_SHAPE Shape;
		DirectX::XMFLOAT3 HalfExtents;
		FLOAT Radius;
		FLOAT Height;
		bool IsSensor;

		Collider()
			: Shape(COLLIDER_SHAPE_BOX)
			, HalfExtents(0.5f, 0.5f, 0.5f)
			, Radius(0.5f)
			, Height(1.0f)
			, IsSensor(false)
		{
		}
	};

	struct AudioSourceComponent
	{
		std::string FileName;
		FLOAT Volume;
		bool PlayOnStart;
		bool Loop;
		bool Playing;

		AudioSourceComponent()
			: Volume(1.0f)
			, PlayOnStart(false)
			, Loop(false)
			, Playing(false)
		{
		}
	};

	struct ScriptComponent
	{
		std::string Path;
		std::string Source;
		bool Enabled;
		std::string LastError;

		ScriptComponent()
			: Enabled(true)
		{
		}
	};

	struct Light
	{
		LIGHT_KIND Kind;
		DirectX::XMFLOAT3 Color;
		FLOAT Intensity;
		FLOAT Range;
		FLOAT SpotAngle;
		bool CastShadows;

		Light()
			: Kind(LIGHT_KIND_DIRECTIONAL)
			, Color(1.0f, 1.0f, 1.0f)
			, Intensity(1.0f)
			, Range(10.0f)
			, SpotAngle(0.5f)
			, CastShadows(true)
		{
		}
	};

	struct PrefabComponent
	{
		std::string PrefabPath;
	};
}

#endif
