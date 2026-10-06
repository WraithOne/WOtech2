---
name: wotech2-engine
description: Build and extend the WOtech2 Windows DX11 engine. Use when adding components, Lua APIs, editor commands, scenes, or tests in this repository.
---

# WOtech2 engine

Follow `AGENTS.md`. Style matches `WO_DeviceDX11.hpp`: banner, `WO_*_H`, `namespace WOtech`, SAL, tabs, `m_` members.

- New public headers go in `Include/` and `WOtech.hpp`. Sources go in `Source/`.
- Extend `Application`, `GameScene`, `ForwardRenderer`, `DeferredRenderer`, and `WO_Network.h`. Do not rename `IGame::Initalize`.
- Components live in `WO_Components.hpp` and are saved by `WO_Scene.cpp`.
- Editor agents use TCP `127.0.0.1:27182`, one JSON object per line. Schema: `docs/EditorCommandAPI.md`.
- Lua must not expose `os.execute`, `io.popen`, or `loadfile`.
- Tests are `WOtech2Tests`. Build Debug|x64 with msbuild v145 and run `bin\x64\Debug\WOtech2Tests.exe` before finishing.
