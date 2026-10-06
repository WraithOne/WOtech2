////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WOtech.hpp
///
///			Description:
///			Main Header file for WOtech engine
///			Includes all main WOtech headers
///			Created:	30.12.2025
///			Edited:		04.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_H
#define WO_H

//////////////
// INCLUDES //
//////////////

// Application / Window / Storage
#include "Include\WO_Application.hpp"
#include "Include\WO_Window.hpp"
#include "Include\WO_Storage.hpp"

// Graphics
#include "Include\WO_DeviceDX11.hpp"
#include "Include\WO_SpriteBatch.hpp"
#include "Include\WO_2DComponents.hpp"
#include "Include\WO_3DComponents.hpp"
#include "Include\WO_VertexTypes.hpp"
#include "Include\WO_Materials.hpp"
#include "Include\WO_RenderCommand.hpp"
#include "Include\WO_ForwardRenderer.hpp"
#include "Include\WO_DeferredRenderer.hpp"

// Audio
#include "Include\WO_Audio.hpp"
#include "Include\WO_AudioComponents.hpp"

// Input
#include "Include\WO_Input.hpp"
#include "Include\WO_VirtualController.hpp"

// Game / Scene
#include "Include\WO_IGame.hpp"
#include "Include\WO_IGameSceneObject.hpp"
#include "Include\WO_GameScene.hpp"
#include "Include\WO_Entity.hpp"
#include "Include\WO_Components.hpp"
#include "Include\WO_World.hpp"
#include "Include\WO_Json.hpp"
#include "Include\WO_Scene.hpp"
#include "Include\WO_EditorCommand.hpp"
#include "Include\WO_Physics.hpp"
#include "Include\WO_Script.hpp"
#include "Include\WO_RenderPipeline.hpp"
#include "Include\WO_Gltf.hpp"

// Network
#include "Include\WO_Network.h"

// Utilities
#include "Include\WO_GameTimer.hpp"
#include "Include\WO_Framecounter.hpp"
#include "Include\WO_Utilities.hpp"

#endif