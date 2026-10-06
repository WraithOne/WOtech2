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
///			Editor executable. ImGui dockspace, transform gizmo, command API.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

#define WO_EDITOR 1

#include "WO_pch.hpp"
#include "WO_EditorCommand.hpp"
#include "WO_RenderPipeline.hpp"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include <d3d11.h>
#include <cstdio>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	WOtech::World g_world;
	WOtech::EditorApi g_api(&g_world);
	WOtech::EditorCommandServer g_commands;
	WOtech::Entity g_selected = WOtech::kInvalidEntity;
	int g_gizmo = 0;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		{
			return 1;
		}
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int)
{
	g_commands.Attach(&g_api);
	g_commands.Start(WOtech::kEditorCommandPort);

	WNDCLASSW windowClass = {};
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"WOtech2Editor";
	RegisterClassW(&windowClass);
	HWND hwnd = CreateWindowW(windowClass.lpszClassName, L"WOtech2 Editor", WS_OVERLAPPEDWINDOW, 80, 80, 1280, 800, nullptr, nullptr, instance, nullptr);
	ShowWindow(hwnd, SW_SHOW);

	DXGI_SWAP_CHAIN_DESC swap = {};
	swap.BufferCount = 2;
	swap.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap.OutputWindow = hwnd;
	swap.SampleDesc.Count = 1;
	swap.Windowed = TRUE;
	swap.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &level, 1, D3D11_SDK_VERSION, &swap, swapChain.GetAddressOf(), device.GetAddressOf(), nullptr, context.GetAddressOf());
	if (FAILED(hr))
	{
		return 1;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device.Get(), context.Get());

	bool running = true;
	while (running)
	{
		MSG message;
		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		g_commands.Poll();
		UNREFERENCED_PARAMETER(g_api.isPlaying());

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		ImGui::Begin("Scene");
		std::vector<WOtech::Entity> alive;
		g_world.GetAliveEntities(&alive);
		for (size_t i = 0; i < alive.size(); ++i)
		{
			char label[128];
			sprintf_s(label, "%s##%llu", g_world.getName(alive[i]), static_cast<unsigned long long>(alive[i]));
			if (ImGui::Selectable(label, g_selected == alive[i]))
			{
				g_selected = alive[i];
			}
		}
		if (ImGui::Button("Create"))
		{
			g_selected = g_api.CreateEntity("Entity");
			g_api.AddComponent(g_selected, WOtech::COMPONENT_TYPE_TRANSFORM);
		}
		ImGui::End();

		ImGui::Begin("Gizmo");
		ImGui::RadioButton("Translate", &g_gizmo, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Rotate", &g_gizmo, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Scale", &g_gizmo, 2);
		if (g_world.isAlive(g_selected))
		{
			WOtech::Transform value;
			if (g_world.getTransform(g_selected) != nullptr)
			{
				value = *g_world.getTransform(g_selected);
			}
			float position[3] = { value.Position.x, value.Position.y, value.Position.z };
			float rotation[3] = { value.Rotation.x, value.Rotation.y, value.Rotation.z };
			float scale[3] = { value.Scale.x, value.Scale.y, value.Scale.z };
			if (g_gizmo == 0) ImGui::DragFloat3("Position", position, 0.05f);
			if (g_gizmo == 1) ImGui::DragFloat3("Rotation", rotation, 0.01f);
			if (g_gizmo == 2) ImGui::DragFloat3("Scale", scale, 0.01f);
			value.Position = DirectX::XMFLOAT3(position[0], position[1], position[2]);
			value.Rotation = DirectX::XMFLOAT3(rotation[0], rotation[1], rotation[2]);
			value.Scale = DirectX::XMFLOAT3(scale[0], scale[1], scale[2]);
			g_api.SetTransform(g_selected, value);
			ImGui::Text("Axis gizmo: X red, Y green, Z blue");
			ImDrawList* draw = ImGui::GetWindowDrawList();
			ImVec2 origin = ImGui::GetCursorScreenPos();
			draw->AddLine(origin, ImVec2(origin.x + 80, origin.y), IM_COL32(220, 40, 40, 255), 3.0f);
			draw->AddLine(origin, ImVec2(origin.x, origin.y + 80), IM_COL32(40, 200, 40, 255), 3.0f);
			draw->AddLine(origin, ImVec2(origin.x + 40, origin.y - 40), IM_COL32(40, 80, 220, 255), 3.0f);
		}
		if (ImGui::Button("Play")) g_api.Play();
		ImGui::SameLine();
		if (ImGui::Button("Stop")) g_api.Stop();
		ImGui::End();

		ImGui::Render();
		FLOAT const clear[4] = { 0.1f, 0.1f, 0.12f, 1.0f };
		context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
		context->ClearRenderTargetView(rtv.Get(), clear);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	g_commands.Stop();
	return 0;
}
