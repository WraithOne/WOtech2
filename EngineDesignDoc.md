# WOtech2 Engine

We're building a production-grade, simple and straight-forward 3D Game Engine for Windows and Microsoft Store. This is professional-grade software that needs to be stable, fully-tested, and real-world deployable. No hacks, no shortcuts - solid as a rock.

## Dev Workflow
- Ask questions only when absolutely necessary - work autonomously
- Testing is of the utmost importance - use lots of unit tests (any framework, simple is good), run automated testing
- Setup a scene used for testing, which tests every single feature in the engine - all components, and entire scripting API
- Do things properly, this is production-grade and not a hack project
- Create AGENTS.md and relevant skills to support development, and contain concrete development guidelines
- Code style to match WO_DeviceDX11.hh and WO_DeviceDX11.cpp
- Use as much of the existing code in this directory as possible.
- When using externel code, only use publicly available open source software
- Create git repository, commit and push to [GitHub repo](https://github.com/WraithOne/WOtech2). IMPORTANT: do a code review before committing, and make sure all changes comply with code style, production-grade quality standards, and have been properly tested/have unit tests that have passed where necessary

## Tech stack
- Visual Studio 2026
- C++ 20
- DirectX 11
- [Windows App SDK](https://github.com/microsoft/WindowsAppSDK)
- [C++/WINRT](https://github.com/microsoft/cppwinrt)
- [imGUI](https://github.com/ocornut/imgui) for UI
- [box3d](https://github.com/erincatto/box3d) for Physic
- Lua for scripting
- Put any used external software in the ThirdParty folder and add it to README.md

## Basic Architecture
- Static library for core engine, executable for editor and runtime (depending on chosen design)
- Need to have editor to build games - this can be embedded into runtime executable (and stripped from distribution builds), or can be a standalone executable
- Simple ECS perhaps, to author scene with entities and components
- Ability to "export" game - executable that runs game without editing ability that we can distribute
- Editor needs to be fully controllable by AI agents - I should be able to ask you to build me a game like Tetris, and you should have all the tools available to do so without my intervention

## 3D Renderer
- Import gltf meshes with materials and textures
- Editor has gizmo to position them in the world
- PBR material workflow
- IBL with HDRIs from https://polyhaven.com/hdris
- Soft shadow maps
- Good SSAO
- HDR pipeline with tonemapping

## 3D Physics
- Controllable via scripting, authored via components in editor

## Network
- Server/Client must be supported

## Controls
- Keyboard&Mouse / Controller / Touch must be supported
- Virtual Controller (WO_VirtualController.hpp/WO_VirtualController.cpp) for interoperability

## Audio
- Basic Audio Input and Output via XAudio2

## Behavior
- Scripting with Lua
- Control entities/components and run in update loop, and entity destruction/creation
- Spawn new entities/prefabs

## Documention
- After finishing create a [Wiki](https://github.com/WraithOne/WOtech2/wiki)