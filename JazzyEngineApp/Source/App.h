#pragma once
#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "Light.h"

using namespace Light;

// App is the main class that starts the game loop/engine

class RenderObj;
class Camera;
class AssetManager;

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
	void LoadMaterials();

	// Start game loop
	int Run();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void RenderFrame();

	void AddRenderObj(RenderObj* obj);

	// Light stuff
	PointLightData* AllocateLight(Vector3 lightPos);
	void FreeLight(PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight() const;

private:
	RenderObj* testCube;

	std::vector<RenderObj*> renderObjects;

	Camera* mCamera;

	AssetManager* mAssetManager;

	LightingConstants mLightConsts = {};

	ID3D11Buffer* lightConstBuffer;

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

