////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: main.cpp
///
///			Description:
///			Distribution runtime. Editor code is not compiled in.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

#define WO_EDITOR 0

#include "WO_pch.hpp"
#include "WO_GameScene.hpp"
#include "WO_RenderPipeline.hpp"
#include "WO_Gltf.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
	const char* scenePath = argc > 1 ? argv[1] : "Assets\\Scenes\\FeatureTest.json";
	WOtech::GameScene scene;
	if (!scene.Load(scenePath))
	{
		std::printf("failed to load %s\n", scenePath);
		return 1;
	}
	UINT32 frames = 60;
	if (argc > 2)
	{
		frames = static_cast<UINT32>(atoi(argv[2]));
	}
	WOtech::GameTime time = {};
	time.DeltaTime = 1.0f / 60.0f;
	for (UINT32 i = 0; i < frames; ++i)
	{
		time.PlayingTime += time.DeltaTime;
		scene.Update(time);
	}
	std::string error;
	bool const rendered = WOtech::RenderPipeline::RenderFeatureFrame(&error);
	std::printf("runtime frames=%u entities=%u rendered=%d %s\n", frames, scene.getWorld().getAliveCount(), rendered ? 1 : 0, error.c_str());
	return 0;
}
