#include "stdafx.h"
#include "App.h"
#include "Graphics.h"
#include "EngineMath.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderObj.h"
#include "Cube.h"
#include "Texture.h"
#include "Camera.h"
#include "AssetManager.h"
#include "Material.h"
#include "Sphere.h"
#include "ModelImporter.h"
#include "Mesh.h"

#define WINWIDTH 1600
#define WINHEIGHT 900

App::App()
	: mCamera(nullptr)
	, mAssetManager(nullptr)
	, lightConstBuffer(nullptr)
{
	wnd = new Window(WINWIDTH, WINHEIGHT, L"Engine");
	running = true;
}

App::~App()
{

}

void App::f()
{
	VertexLayout v1;
	v1.Append<VertexLayout::ElementType::Pos3D>();
	v1.Append<VertexLayout::ElementType::Normal>();
	v1.Append<VertexLayout::ElementType::Texture2D>();

	v1.Size();

	VBuffer vb(std::move(v1));

	vb.Size();

	vb.EmplaceBack(Vector3(1.0f, 1.0f, 5.0f), Vector3(2.0f, 1.0f, 4.0f), Vector2(6.0f,9.0f));
	vb.EmplaceBack(Vector3(6.0f, 9.0f, 6.0f), Vector3(9.0f, 6.0f, 9.0f), Vector2(4.2f, 0.0f));
	Vector3 pos = vb[0].Attr<VertexLayout::ElementType::Pos3D>();
	Vector3 nor = vb[0].Attr<VertexLayout::ElementType::Normal>();
	Vector2 tex = vb[1].Attr<VertexLayout::ElementType::Texture2D>();
	vb.Back().Attr<VertexLayout::ElementType::Pos3D>().z = 420.0f;
	pos = vb.Back().Attr<VertexLayout::ElementType::Pos3D>();
	vb.SizeBytes();

	const auto& cvb = vb;
	pos = cvb[1].Attr<VertexLayout::ElementType::Pos3D>();
}

void App::Init()
{
	mCamera = new Camera();

	mAssetManager = new AssetManager();

	ModelImporter mModImp;

	lightConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mLightConsts,
		sizeof(mLightConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	const VertexPosNormUV vTexture[] =
	{
		// Front
		{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f)},

		// Right
		{ Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f,0.0f,0.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f,0.0f,0.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f,0.0f,0.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f,0.0f,0.0f), Vector2(0.0f, 1.0f)},
		
		// Back
		{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f,0.0f,1.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f,0.0f,1.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f,0.0f,1.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f,0.0f,1.0f), Vector2(0.0f, 1.0f)},

		// Left
		{ Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)},

		// Bottom
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)},

		// Top
		{ Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)},
		{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)},
		{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)},
	};
	const Vertex vColor[] =
	{
		// Front
		{ Vector3(-1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },

		// Right
		{ Vector3(1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },

		// Back
		{ Vector3(1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },

		// Left
		{ Vector3(-1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },

		// Bottom
		{ Vector3(-1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },

		// Top
		{ Vector3(-1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f), Color4(0.7f, 0.7f, 0.7f, 1.0f) },
	};

	const uint16_t indices[] =
	{
		1, 2, 0,
		2, 3, 0,

		7, 4, 6,
		4, 5, 6,

		9, 10, 8,
		10, 11, 8,

		13, 14, 12,
		14, 15, 12,

		17, 18, 16,
		18, 19, 16,
		
		21, 22, 20,
		22, 23, 20,
	};

	mAssetManager->LoadTexture("Assets/Textures/hoovy.jpg");

	LoadShaders();

	LoadMaterials();

	std::vector<Mesh*> nanoMesh = mModImp.CreateModel("Assets/Models/Falco/falco.obj");
	RenderObj* nano = new RenderObj(nanoMesh);
	AddRenderObj(nano);
	nano->SetScale(0.04f);
	nano->SetPos(Vector3(0.0f, -1.5f, 1.0f));
	nano->SetYaw(Math::Pi);

	std::vector<Mesh*> squidMesh = mModImp.CreateModel("Assets/Models/Squidward/squidward.obj");
	RenderObj* squid = new RenderObj(squidMesh);
	AddRenderObj(squid);
	squid->SetScale(0.3f);
	squid->SetPos(Vector3(-5.0f, -1.5f, 1.0f));
	squid->SetYaw(Math::Pi);

	// Set ambient light
	SetAmbientLight(Vector3(0.06f,0.06f,0.06f));

	PointLightData* light1 = AllocateLight(Vector3(0.0f,5.0f,-5.0f));
	light1->lightColor = Vector3(1.0f, 1.0f, 1.0f);
	light1->innerRadius = 20.0f;
	light1->outerRadius = 200.0f;

	PointLightData* light2 = AllocateLight(Vector3(22.0f,15.0f,25.0f));
	light2->lightColor = Vector3(0.2f, 0.7f, 0.9f);
	light2->innerRadius = 20.0f;
	light2->outerRadius = 50.0f;
}

void App::ShutDown()
{
	while (!renderObjects.empty())
	{
		delete renderObjects.back();
		renderObjects.pop_back();
	}

	if (mCamera)
	{
		delete mCamera;
	}

	if (lightConstBuffer)
	{
		lightConstBuffer->Release();
	}

	if (mAssetManager)
	{
		delete mAssetManager;
	}

	// Delete/release stuff before window
	if (wnd)
	{
		delete wnd;
	}
}

void App::LoadShaders()
{
	// Simple shader of just position and color
	Shader* simple = new Shader();
	const D3D11_INPUT_ELEMENT_DESC simpleIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	simple->Load(L"Shaders/SimpleVS.hlsl", ShaderType::Vertex, simpleIed, sizeof(simpleIed) / sizeof(simpleIed[0]));
	simple->Load(L"Shaders/SimplePS.hlsl", ShaderType::Pixel, simpleIed, sizeof(simpleIed) / sizeof(simpleIed[0]));
	mAssetManager->SaveShader("Simple", simple);

	Shader* simpleLit = new Shader();
	const D3D11_INPUT_ELEMENT_DESC simpleLitIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	simpleLit->Load(L"Shaders/SimpleLitVS.hlsl", ShaderType::Vertex, simpleLitIed, sizeof(simpleLitIed) / sizeof(simpleLitIed[0]));
	simpleLit->Load(L"Shaders/SimpleLitPS.hlsl", ShaderType::Pixel, simpleLitIed, sizeof(simpleLitIed) / sizeof(simpleLitIed[0]));
	mAssetManager->SaveShader("SimpleLit", simpleLit);

	// Shader for textured objects
	Shader* texturedShader = new Shader();
	const D3D11_INPUT_ELEMENT_DESC texturedIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	texturedShader->Load(L"Shaders/TexturedVS.hlsl", ShaderType::Vertex, texturedIed, sizeof(texturedIed) / sizeof(texturedIed[0]));
	texturedShader->Load(L"Shaders/TexturedPS.hlsl", ShaderType::Pixel, texturedIed, sizeof(texturedIed) / sizeof(texturedIed[0]));
	mAssetManager->SaveShader("Textured", texturedShader);

	// Phong Shader for textured objects + phong lighting
	Shader* phongShader = new Shader();
	const D3D11_INPUT_ELEMENT_DESC phongIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	phongShader->Load(L"Shaders/PhongVS.hlsl", ShaderType::Vertex, phongIed, sizeof(phongIed) / sizeof(phongIed[0]));
	phongShader->Load(L"Shaders/PhongPS.hlsl", ShaderType::Pixel, phongIed, sizeof(phongIed) / sizeof(phongIed[0]));
	mAssetManager->SaveShader("Phong", phongShader);

	// Phong with no texture, but vertex colors
	Shader* colorPhong = new Shader();
	const D3D11_INPUT_ELEMENT_DESC colorPhongIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	colorPhong->Load(L"Shaders/VertexShader.hlsl", ShaderType::Vertex, colorPhongIed, sizeof(colorPhongIed) / sizeof(colorPhongIed[0]));
	colorPhong->Load(L"Shaders/PixelShader.hlsl", ShaderType::Pixel, colorPhongIed, sizeof(colorPhongIed) / sizeof(colorPhongIed[0]));
	mAssetManager->SaveShader("ColorPhong", colorPhong);
}

void App::LoadMaterials()
{
	// Initialize the materials for objects
	Material* pointLightMat = new Material();
	pointLightMat->SetShader(mAssetManager->GetShader("Simple"));
	mAssetManager->SaveMaterial("PointLight", pointLightMat);

	Material* monkeMat = new Material();
	monkeMat->SetShader(mAssetManager->GetShader("SimpleLit"));
	monkeMat->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
	monkeMat->SetSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
	monkeMat->SetSpecularPower(100.0f);
	mAssetManager->SaveMaterial("Monke", monkeMat);

	Material* coloredCubeMat = new Material();
	coloredCubeMat->SetShader(mAssetManager->GetShader("ColorPhong"));
	coloredCubeMat->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
	coloredCubeMat->SetSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
	coloredCubeMat->SetSpecularPower(10.0f);
	mAssetManager->SaveMaterial("ColoredCube", coloredCubeMat);

	Material* phongMat = new Material();
	phongMat->SetShader(mAssetManager->GetShader("Phong"));
	phongMat->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
	phongMat->SetSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
	phongMat->SetSpecularPower(100.0f);
	mAssetManager->SaveMaterial("Phong", phongMat);
}

int App::Run()
{
	// Initialize all game objects, shaders, etc.
	Init();

	    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); //(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(wnd->GetHwnd());
    ImGui_ImplDX11_Init(Graphics::Get()->GetDevice(), Graphics::Get()->GetContext());

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	MSG msg = {};

	static float f = 1.0f;
	static int counter = 0;

	// Get the current time
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	while (running)
	{
		// Check to see if any messages are waiting in the queue
		while ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message)
			{
			case WM_QUIT:
				running = false;
				break;
			}
		}

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float deltaTime = (float)(0.000000001 * duration);
		start = end;

		fps = (int)(1.0f / deltaTime);

		
		float camPanSpeed = 3.0f;
		camPanSpeed /= f;

		if (isPaused)
		{
			f = 0.0f;
		}
		else
		{
			f = prevSpeed;
		}

		ProcessInput(deltaTime);

		Update(deltaTime);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::Begin("Camera");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("Use arrow keys to move camera");               // Display some text (you can use a format strings too)
			ImGui::Text("Use WASD to pan camera (in first person)");
			ImGui::Text("Use Z and X to zoom in and out");
			ImGui::Text("Use Q and E to roll");
			ImGui::Text("");
			
			std::string type = "";
			if (mCamera->GetType() == CameraType::FirstPerson)
			{
				type = "First Person";
			}
			else
			{
				type = "Third Person";
			}

			ImGui::Text("Camera Type: %s", type.c_str());

			ImGui::Text("Cam Fwd: %.2f, %.2f, %.2f", mCamera->cameraForward.x, mCamera->cameraForward.y, mCamera->cameraForward.z);
			ImGui::Text("Cam Rot: yaw = %i, pitch = %i, roll = %i", 
				(int)Math::ToDegrees(mCamera->yaw) % 360,
				(int)Math::ToDegrees(mCamera->pitch) % 360, 
				(int)Math::ToDegrees(mCamera->roll) % 360);
			
			//ImGui::Text("Cube pos: %.2f,%.2f,%.2f", testCube->GetPos().x, testCube->GetPos().y, testCube->GetPos().z);

			ImGui::Text("Camera pos: %.2f,%.2f,%.2f", mCamera->mCamConsts.position.x, mCamera->mCamConsts.position.y, mCamera->mCamConsts.position.z);
		
			ImGui::End();
		}
		// 3. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::Begin("Game");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);
			
			ImGui::SliderFloat("Game Speed", &prevSpeed, 0.05f, 2.5f);            // Edit 1 float using a slider from 0.0f to 1.0f
			
			ImGui::SameLine();
			if (ImGui::Button("Reset"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				f = 1.0f;
				prevSpeed = 1.0f;
				mCamera->ResetCamera();
			}
			
			float msf = 1000.0f * deltaTime;
			ImGui::Text("Application average %.3f ms/frame (%i FPS)", msf, fps);
			ImGui::End();
		}

		// 4. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();

		RenderFrame();
		
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		Graphics::Get()->EndFrame();
	}

	ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	ShutDown();

	return (int)msg.wParam;
}

void App::ProcessInput(float deltaTime)
{
	while (!wnd->mMouse->IsEmpty())
	{
		const auto e = wnd->mMouse->Read();
		/*switch (e.GetType())
		{
		case Mouse::Event::Type::WheelUp:
			zoom += 0.2f;
			break;
		case Mouse::Event::Type::WheelDown:
			zoom -= 0.2f;
			break;
		}*/
	}
	if (!isPaused)
	{
		if (mCamera->GetType() == CameraType::FirstPerson)
		{
			if (wnd->mKeyboard->KeyIsPressed('W'))
			{
				mCamera->mCamConsts.position += Vector3(0.0f, 1.0f, 0.0f) * deltaTime * 7.5f;
			}
			if (wnd->mKeyboard->KeyIsPressed('S'))
			{
				mCamera->mCamConsts.position += Vector3(0.0f, -1.0f, 0.0f) * deltaTime * 7.5f;
			}
			if (wnd->mKeyboard->KeyIsPressed('D'))
			{
				mCamera->mCamConsts.position += Vector3(1.0f, 0.0f, 0.0f) * deltaTime * 7.5f;
			}
			if (wnd->mKeyboard->KeyIsPressed('A'))
			{
				mCamera->mCamConsts.position += Vector3(-1.0f, 0.0f, 0.0f) * deltaTime * 7.5f;
			}
			if (wnd->mKeyboard->KeyIsPressed('E'))
			{
				mCamera->roll += deltaTime * 3.0f;
			}
			if (wnd->mKeyboard->KeyIsPressed('Q'))
			{
				mCamera->roll -= deltaTime * 3.0f;
			}
		}
		if (wnd->mKeyboard->KeyIsPressed('Z'))
		{
			mCamera->mCamConsts.position += mCamera->cameraForward * deltaTime * 7.5f;
		}
		if (wnd->mKeyboard->KeyIsPressed('X'))
		{
			mCamera->mCamConsts.position -= mCamera->cameraForward * deltaTime * 7.5f;
		}
		if (wnd->mKeyboard->KeyIsPressed(VK_LEFT))
		{
			mCamera->yaw += deltaTime * 2.0f;
		}
		if (wnd->mKeyboard->KeyIsPressed(VK_RIGHT))
		{
			mCamera->yaw -= deltaTime * 2.0f;
		}
		if (wnd->mKeyboard->KeyIsPressed(VK_UP) && mCamera->pitch > -Math::Pi / 2.01f)
		{
			mCamera->pitch -= deltaTime * 2.0f;
		}
		if (wnd->mKeyboard->KeyIsPressed(VK_DOWN) && mCamera->pitch < Math::Pi / 2.01f)
		{
			mCamera->pitch += deltaTime * 2.0f;
		}
		if (wnd->mKeyboard->KeyIsPressed('V') && !prevCam)
		{
			mCamera->SwitchCamera();
		}
	}
	if (wnd->mKeyboard->KeyIsPressed(VK_ESCAPE) && !prevSim)
	{
		isPaused = !isPaused;
	}
	prevCam = wnd->mKeyboard->KeyIsPressed('V');
	prevSim = wnd->mKeyboard->KeyIsPressed(VK_ESCAPE);
}

void App::Update(float deltaTime)
{
	for (auto o : renderObjects)
	{
		o->Update(deltaTime);
	}
}

void App::RenderFrame()
{
	Graphics* g = Graphics::Get();

	// set render target
	g->SetBuffer(g->GetBackBuffer(), g->GetDepthStencilView());

	{
		// clear buffers
		g->ClearBuffer(0.08f, 0.08f, 0.08f);
		g->ClearDepthBuffer(g->GetDepthStencilView(), 1.0f);
	}

	mCamera->SetActive();

	// Upload and bind lighting constants to the pixel shader
	g->UploadBuffer(lightConstBuffer, &mLightConsts, sizeof(mLightConsts));
	g->GetContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTING, 1, &lightConstBuffer);

	for (auto o : renderObjects)
	{
		o->Draw();
	}
}

void App::AddRenderObj(RenderObj* obj)
{
	renderObjects.push_back(obj);
}

PointLightData* App::AllocateLight(Vector3 lightPos)
{
	// Create a sphere set its material for the point light
	Sphere* sphere = new Sphere();
	AddRenderObj(sphere);
	sphere->SetPos(lightPos);

	PointLightData* returnLight = nullptr;

	for (int i = 0; i < ARRAY_SIZE(mLightConsts.pointLight); i++)
	{
		if (!mLightConsts.pointLight[i].isEnabled)
		{
			mLightConsts.pointLight[i].isEnabled = true;
			returnLight = &mLightConsts.pointLight[i];
			returnLight->position = lightPos;
			return returnLight;
		}
	}
	return returnLight;
}

void App::FreeLight(PointLightData* pLight)
{
	pLight->isEnabled = false;
}

void App::SetAmbientLight(const Vector3& color)
{
	mLightConsts.ambientColor = color;
}

const Vector3& App::GetAmbientLight() const
{
	return mLightConsts.ambientColor;
}
