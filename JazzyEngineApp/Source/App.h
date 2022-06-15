#pragma once
#include "Window.h"

// App is the main class that starts the game loop/engine

class RenderObj;
class Texture;
class Graphics;

class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void Init(HWND hwnd, float width, float height);
	void ShutDown();

	// Start game loop
	int Run();

	void Update(float deltaTime);
	void RenderFrame();

	void AddRenderObj(RenderObj* obj);

private:
	RenderObj* testCube;

	std::vector<RenderObj*> renderObjects;

	ID3D11Buffer* mConstColorBuffer;

	Texture* hoovy = nullptr;

	float angle = 0.0f;
	float zoom = 0.0f;

	int fps = 0;

	bool running;

	Graphics* mGraphics;

	HWND hWnd;

	//Window* wnd;
};

