# Architecture

`WOtech2` is a static library. `WOtech2Editor` defines `WO_EDITOR=1` and hosts Dear ImGui plus the command server. `WOtech2Runtime` is built without the editor and loads an exported scene.

`Application` ticks `IGame`. `GameScene` owns a `World`, steps `PhysicsWorld`, then ticks `ScriptHost`. Rendering goes through `DeviceDX11` and `ForwardRenderer`. `RenderPipeline` draws HDR, SSAO, a soft shadow map, and tonemaps into an 8-bit target so the existing swap chain stays `B8G8R8A8_UNORM`.
