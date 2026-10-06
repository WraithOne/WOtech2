# WOtech2

Windows and Microsoft Store 3D game engine. DirectX 11, C++20, Windows App SDK, Visual Studio 2026 (toolset v145). The core is the static library `WOtech2`. `WOtech2Editor` authors games. `WOtech2Runtime` is the editor-free distribution executable (`WO_EDITOR=0`).

## Build

Restore NuGet, then build x64:

```
.tools\nuget.exe restore packages.config -PackagesDirectory packages
msbuild WOtech2.sln /p:Configuration=Debug /p:Platform=x64
```

`packages.config` lists the Windows App SDK packages. `packages/` is not committed.

## Run

- Tests: `bin\x64\Debug\WOtech2Tests.exe`
- Editor: `bin\x64\Debug\WOtech2Editor.exe` (command API on `127.0.0.1:27182`)
- Runtime: `bin\x64\Debug\WOtech2Runtime.exe Assets\Scenes\FeatureTest.json 30`
- Export: `powershell -File scripts\Export-Game.ps1`

## Third party

| Name | URL | License | Pin |
| --- | --- | --- | --- |
| Dear ImGui docking | https://github.com/ocornut/imgui | MIT | `0f4b927e819823006a878bcb7fe93be129fd8d3f` |
| box3d | https://github.com/erincatto/box3d | MIT | `16f7f4cf4c3d6579e14e4309aa08f878cd59a520` |
| Lua 5.4.7 | https://www.lua.org/ | MIT | `1ab3208a1fceb12fca8f24ba57d6e13c5bff15e3` |
| cgltf 1.14 | https://github.com/jkuhlmann/cgltf | MIT | v1.14 |
| WICTextureLoader, MediaReader, FontFileLoader | existing `ThirdParty/` | see `ThirdParty/README.md` | existing |

Production HDRIs: https://polyhaven.com/hdris. This repo ships only a small generated sample under `Assets/HDRI`. The feature frame builds an irradiance cubemap, a prefiltered specular cubemap, and a BRDF LUT from that sample, samples them in the PBR shader, and applies a 3x3 PCF shadow of the feature mesh. HDR, SSAO, and Reinhard tonemap stay on the same path.

## Layout

`Include/` and `Source/` are the engine. `Editor/` and `Runtime/` are the executables. `docs/` is the wiki source. `AGENTS.md` is the contributor and agent guide.
