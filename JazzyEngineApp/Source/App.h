#pragma once
#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

// App is the main class that starts the game loop/engine

class RenderObj;
class Cube;
class Texture;
class Camera;
class AssetManager;
class Material;

class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void Init();
	void ShutDown();

	void LoadShaders();

	// Start game loop
	int Run();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void RenderFrame();

	void AddRenderObj(RenderObj* obj);

private:
	RenderObj* testCube;

	std::vector<RenderObj*> renderObjects;

	ID3D11Buffer* mConstColorBuffer;

	Camera* mCamera;

	Texture* hoovy = nullptr;

	AssetManager* mAssetManager;

	Material* phongMaterial;
	Material* phongTexturedMaterial;

	float angle = 0.0f;
	float zoom = 1.0f;

	bool prevCam = false;
	bool prevSim = false;

	bool isPaused = false;
	float prevSpeed = 1.0f;

	int fps = 0;

	bool running;

	Window* wnd;
};

