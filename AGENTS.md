# Agent guidelines

Match `Include/WO_DeviceDX11.hpp` and `Source/WO_DeviceDX11.cpp`: banner, `WO_*_H` guards, `namespace WOtech`, SAL, `m_` members, tabs, PascalCase operations, `set*` / `get*` accessors.

Do not rewrite `DeviceDX11`, input, audio, sprite batch, materials, or existing shaders unless a change is required to integrate a system. Extend `Application`, `GameScene`, `ForwardRenderer`, `DeferredRenderer`, and `Network`. Keep the `IGame::Initalize` spelling.

## Add a component

1. Add the struct to `Include/WO_Components.hpp` and a `COMPONENT_TYPE` value.
2. Store it on `World` with `Add*` / `get*` / `Remove*`.
3. Write it in `Source/WO_Scene.cpp`.
4. Teach `EditorApi::AddComponent` and `docs/EditorCommandAPI.md`.
5. If Lua should see it, add a function in `Source/WO_Script.cpp` and do not open `os`, `io`, or `package`.

## Drive the editor

Connect to `127.0.0.1:27182` and send one JSON command per line. Schema: `docs/EditorCommandAPI.md`. Prefer that API over clicking. A Tetris-style game is a scene of entities, a script component, and `play`.

## Build and test

Visual Studio 2026, toolset v145, x64. Restore NuGet with `packages.config`, then:

```
msbuild WOtech2.sln /p:Configuration=Debug /p:Platform=x64
bin\x64\Debug\WOtech2Tests.exe
```

Editor: `bin\x64\Debug\WOtech2Editor.exe`. Runtime: `bin\x64\Debug\WOtech2Runtime.exe`. Export: `scripts\Export-Game.ps1`.

## Do not

Do not replace XAudio2, add keyboard hooks, bind privileged ports, or vendor closed-source UI. Production HDRIs come from https://polyhaven.com/hdris; do not download them in bulk.
