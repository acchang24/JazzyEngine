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

#define WINWIDTH 1280
#define WINHEIGHT 720

App::App()
	: testCube(nullptr)
	, mConstColorBuffer(nullptr)
	, mCamera(nullptr)
{
	wnd = new Window(WINWIDTH, WINHEIGHT, L"Engine");
	running = true;
}

App::~App()
{

}

void App::Init()
{
	mCamera = new Camera();

	const VertexTexture vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f},
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f},

		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f},
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f},

		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f},

		
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f},
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f},

		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f},
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f},
		{ -1.0f, -1.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f},
	};

	const uint16_t indices[] =
	{
		0, 2, 1,
		2, 3, 1,

		4, 6, 5,
		6, 7, 5,

		8, 10, 9,
		10, 11, 9,

		12, 14, 13,
		14, 15, 13,

		16, 18, 17,
		18, 19, 17,

		20, 22, 21,
		22, 23, 21
	};

	hoovy = new Texture();
	hoovy->Load(L"Assets/Textures/hoovy.jpg");

	// Shader
	Shader* mShader = new Shader();

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const D3D11_INPUT_ELEMENT_DESC tex[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mShader->Load(L"Shaders/TexturedVS.hlsl", ShaderType::Vertex, tex, sizeof(tex) / sizeof(tex[0]));
	mShader->Load(L"Shaders/TexturedPS.hlsl", ShaderType::Pixel, tex, sizeof(tex) / sizeof(tex[0]));

	// Create a render objects
	testCube = new RenderObj(new VertexBuffer(vertices, sizeof(vertices), sizeof(VertexTexture), indices, sizeof(indices), sizeof(uint16_t)), mShader);
	AddRenderObj(testCube);
	testCube->SetPos(Vector3(0.0f,0.0f, 1.0f));
	
	for (int i = 0; i < 80; i++)
	{
		Cube* newCube = new Cube();
		AddRenderObj(newCube);
	}

}

void App::ShutDown()
{
	while (!renderObjects.empty())
	{
		delete renderObjects.back();
		renderObjects.pop_back();
	}

	if (mConstColorBuffer)
	{
		mConstColorBuffer->Release();
	}

	if (mCamera)
	{
		delete mCamera;
	}

	delete hoovy;

	// Delete/release stuff before window
	if (wnd)
	{
		delete wnd;
	}
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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

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

		while (!wnd->mMouse->IsEmpty())
		{
			const auto e = wnd->mMouse->Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::WheelUp:
				zoom += 0.2f;
				break;
			case Mouse::Event::Type::WheelDown:
				zoom -= 0.2f;
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
		Vector3 q = mCamera->mCamConsts.position;
		q.Normalize();
		Vector3 t = testCube->GetPos();
		t.Normalize();
		float dot = Dot(q, t);

		ProcessInput(deltaTime / f);

		Update(deltaTime * f);

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

			ImGui::Text("Zoom: %f", zoom);
			ImGui::Text("Dot angle: %f", dot);

			ImGui::Text("Cam Fwd: %.2f, %.2f, %.2f", mCamera->cameraForward.x, mCamera->cameraForward.y, mCamera->cameraForward.z);
			ImGui::Text("Cam Rot: yaw = %i, pitch = %i, roll = %i", 
				(int)Math::ToDegrees(mCamera->yaw) % 360,
				(int)Math::ToDegrees(mCamera->pitch) % 360, 
				(int)Math::ToDegrees(mCamera->roll) % 360);
			
			ImGui::Text("Cube pos: %.2f,%.2f,%.2f", testCube->GetPos().x, testCube->GetPos().y, testCube->GetPos().z);

			ImGui::Text("Camera pos: %.2f,%.2f,%.2f", mCamera->mCamConsts.position.x, mCamera->mCamConsts.position.y, mCamera->mCamConsts.position.z);
		
			ImGui::End();
		}
		// 3. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::Begin("Game");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);
			
			ImGui::SliderFloat("Simulation Speed", &prevSpeed, 0.05f, 2.5f);            // Edit 1 float using a slider from 0.0f to 1.0f
			if (simStopped)
			{
				f = 0.0f;
			}
			else
			{
				f = prevSpeed;
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Reset"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				f = 1.0f;
				prevSpeed = 1.0f;
				testCube->SetPos(Vector3(0.0f, 0.0f, 1.0f));
				zoom = 1.0f;
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
		mCamera->pitch += deltaTime * 2.0f ;
	}
	if (wnd->mKeyboard->KeyIsPressed('V') && !prevCam)
	{
		mCamera->SwitchCamera();
	}
	if (wnd->mKeyboard->KeyIsPressed(VK_SPACE) && !prevSim)
	{
		simStopped = !simStopped;
	}
	prevCam = wnd->mKeyboard->KeyIsPressed('V');
	prevSim = wnd->mKeyboard->KeyIsPressed(VK_SPACE);
}

void App::Update(float deltaTime)
{
	angle += Math::Pi * deltaTime;

	for (auto o : renderObjects)
	{
		o->Update(deltaTime);
	}

	Matrix4 test = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 10.0f));
	test.Invert();
	Vector3 vhat = test.GetTranslation();

	/*testCube->SetPitch(angle * 0.25f);
	testCube->SetRoll(0.0f);
	testCube->SetYaw(angle);

	testCube->SetPos(Vector3(testCube->GetPos().x, testCube->GetPos().y, zoom));
	Vector3 whtathefuck = testCube->GetPos();

	Matrix4 transform = Matrix4::CreateScale(testCube->GetScale())
		* Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle)
		* Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, zoom + 0.0f));
		
	testCube->mObjConsts.modelToWorld = transform;*/
}

void App::RenderFrame()
{
	Graphics* g = Graphics::Get();

	// set render target
	g->SetBuffer(g->GetBackBuffer(), g->GetDepthStencilView());

	{
		// clear buffers
		g->ClearBuffer(0.0f, 0.0f, 0.0f);

		g->ClearDepthBuffer(g->GetDepthStencilView(), 1.0f);
	}

	mCamera->SetActive();

	hoovy->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);

	for (auto o : renderObjects)
	{
		o->Draw();
	}
}

void App::AddRenderObj(RenderObj* obj)
{
	renderObjects.push_back(obj);
}
