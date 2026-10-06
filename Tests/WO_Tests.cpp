////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Tests.cpp
///
///			Description:
///			In-repo unit tests. No third-party test framework.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

#include "WO_pch.hpp"
#include "WO_World.hpp"
#include "WO_Scene.hpp"
#include "WO_EditorCommand.hpp"
#include "WO_Network.h"
#include "WO_Window.hpp"
#include "WO_Input.hpp"
#include "WO_VirtualController.hpp"
#include "WO_Physics.hpp"
#include "WO_Script.hpp"

#include <cstdio>
#include <string>

namespace
{
	int g_failed = 0;

	void Check(_In_ bool condition, _In_z_ const char* name)
	{
		if (condition)
		{
			std::printf("PASS %s\n", name);
		}
		else
		{
			std::printf("FAIL %s\n", name);
			g_failed += 1;
		}
	}

	void TestEcs()
	{
		WOtech::World world;
		WOtech::Entity const a = world.CreateEntity("a");
		WOtech::Entity const b = world.CreateEntity("b");
		Check(world.isAlive(a) && world.isAlive(b) && a != b, "ecs create distinct");
		WOtech::Transform transform;
		transform.Position = DirectX::XMFLOAT3(1.0f, 2.0f, 3.0f);
		Check(world.AddTransform(a, transform) != nullptr, "ecs add transform");
		Check(world.getTransform(a) != nullptr && world.getTransform(a)->Position.y == 2.0f, "ecs get transform");
		world.DestroyEntity(a);
		Check(!world.isAlive(a) && world.getTransform(a) == nullptr, "ecs destroy");
		WOtech::Entity const c = world.CreateEntity("c");
		Check(c != a && world.isAlive(b) && world.isAlive(c), "ecs stable id recycle");
		Check(world.getAliveCount() == 2, "ecs alive count");
	}

	void TestPrefab()
	{
		WOtech::Prefab prefab;
		prefab.setName("crate");
		WOtech::Transform transform;
		transform.Position = DirectX::XMFLOAT3(4.0f, 5.0f, 6.0f);
		prefab.setTransform(transform);
		WOtech::World world;
		WOtech::Entity const spawned = world.SpawnPrefab(prefab);
		Check(world.isAlive(spawned), "prefab spawn");
		Check(std::string(world.getName(spawned)) == "crate", "prefab name");
		Check(world.getTransform(spawned) != nullptr && world.getTransform(spawned)->Position.x == 4.0f, "prefab transform");
	}

	void TestScene()
	{
		char dir[MAX_PATH];
		GetTempPathA(MAX_PATH, dir);
		std::string path = std::string(dir) + "wotech2-scene-test.json";
		WOtech::World world;
		WOtech::Entity const entity = world.CreateEntity("lamp");
		WOtech::Transform transform;
		transform.Position = DirectX::XMFLOAT3(8.0f, 0.5f, -2.0f);
		world.AddTransform(entity, transform);
		WOtech::Light light;
		light.Intensity = 3.5f;
		world.AddLight(entity, light);
		std::string error;
		Check(WOtech::SceneDocument::Save(world, path.c_str(), &error), "scene save");
		WOtech::World loaded;
		Check(WOtech::SceneDocument::Load(&loaded, path.c_str(), &error), "scene load");
		std::vector<WOtech::Entity> alive;
		loaded.GetAliveEntities(&alive);
		Check(alive.size() == 1, "scene entity count");
		Check(!alive.empty() && loaded.getTransform(alive[0]) != nullptr && loaded.getTransform(alive[0])->Position.x == 8.0f, "scene transform roundtrip");
		Check(!alive.empty() && loaded.getLight(alive[0]) != nullptr && loaded.getLight(alive[0])->Intensity == 3.5f, "scene light roundtrip");
		DeleteFileA(path.c_str());
	}

	void TestVirtualController()
	{
		WOtech::Window window;
		WOtech::InputManager input(&window);
		WOtech::VirtualController controller(&input);
		controller.bindKeyboardtoButton(WOtech::VIRTUAL_CONTROLLER_BUTTONS_A, winrt::Windows::System::VirtualKey::Space);
		winrt::Windows::System::VirtualKey key = winrt::Windows::System::VirtualKey::None;
		Check(controller.getKeyboardButtonBinding(WOtech::VIRTUAL_CONTROLLER_BUTTONS_A, &key), "virtual controller binding stored");
		Check(key == winrt::Windows::System::VirtualKey::Space, "virtual controller maps A to Space");
		controller.setCurrentInput(WOtech::CURRENT_INPUT_DEVICE_KEYBOARDANDMOUSE);
		WOtech::Virtual_Controller_State const state = controller.getState();
		Check(state.Button_A == false, "virtual controller idle A is up");
	}

	void TestNetwork()
	{
		WOtech::Server server;
		server.Start(0);
		Check(server.isListening(), "network server listen");
		WOtech::Client client;
		client.Connect("127.0.0.1", server.getPort());
		Check(client.WaitConnected(2000), "network client connect");
		char const* payload = "hello-wotech2";
		client.Send(payload, static_cast<UINT32>(strlen(payload)));
		bool received = false;
		for (int i = 0; i < 50 && !received; ++i)
		{
			server.Poll();
			client.Poll();
			std::vector<UINT8> packet;
			if (server.TryReceive(&packet))
			{
				received = packet.size() == strlen(payload) && memcmp(packet.data(), payload, packet.size()) == 0;
			}
			Sleep(10);
		}
		Check(received, "network loopback packet");
		client.Disconnect();
		server.Stop();
	}

	void TestPhysics()
	{
		WOtech::World world;
		WOtech::Entity const entity = world.CreateEntity("ball");
		WOtech::Transform transform;
		transform.Position = DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f);
		world.AddTransform(entity, transform);
		WOtech::RigidBody body;
		body.Type = WOtech::BODY_TYPE_DYNAMIC;
		body.UseGravity = true;
		world.AddRigidBody(entity, body);
		WOtech::Collider collider;
		collider.Shape = WOtech::COLLIDER_SHAPE_SPHERE;
		collider.Radius = 0.5f;
		world.AddCollider(entity, collider);
		WOtech::PhysicsWorld physics;
		for (int i = 0; i < 30; ++i)
		{
			physics.Step(&world, 1.0f / 60.0f);
		}
		Check(world.getTransform(entity)->Position.y < 5.0f, "physics wrapper step falls");
	}

	void TestLua()
	{
		WOtech::World world;
		WOtech::ScriptHost host;
		std::string error;
		bool const ok = host.RunSource(&world, "local id = world.create('pawn')\nworld.set_position(id, 3, 4, 5)\nworld.destroy(id)\n", &error);
		Check(ok, "lua create set destroy");
		Check(world.getAliveCount() == 0, "lua destroyed entity");
		bool const blocked = host.RunSource(&world, "os.execute('cmd')", &error);
		Check(!blocked, "lua sandbox rejects os.execute");
	}

	void TestEditorCommand()
	{
		WOtech::World world;
		WOtech::EditorApi api(&world);
		WOtech::EditorCommandServer server;
		server.Attach(&api);
		std::string response = server.Execute("{\"cmd\":\"create_entity\",\"name\":\"pawn\"}");
		Check(response.find("\"ok\":true") != std::string::npos, "editor create entity");
		Check(world.getAliveCount() == 1, "editor entity landed");
		response = server.Execute("{\"cmd\":\"query_scene\"}");
		Check(response.find("pawn") != std::string::npos, "editor query scene");
	}
}

int main()
{
	TestEcs();
	TestPrefab();
	TestScene();
	TestVirtualController();
	TestNetwork();
	TestPhysics();
	TestLua();
	TestEditorCommand();
	std::printf("%s (%d failed)\n", g_failed == 0 ? "ALL PASSED" : "FAILED", g_failed);
	return g_failed == 0 ? 0 : 1;
}
