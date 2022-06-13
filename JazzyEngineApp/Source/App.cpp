#include "stdafx.h"
#include "App.h"
#include <iomanip>
#include "Graphics.h"
#include "EngineMath.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderObj.h"
#include "Cube.h"
#include "Texture.h"

#define WINWIDTH 1280
#define WINHEIGHT 720

App::App()
	: testCube(nullptr)
	, mConstColorBuffer(nullptr)
{
	wnd = new Window(WINWIDTH, WINHEIGHT, L"Engine");
	running = true;
}

App::~App()
{

}

void App::Init()
{
	const VertexTexture vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f},
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f},
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
	};

	const uint16_t indices[] =
	{
		0, 2, 1,
		2, 3, 1,
		1, 3, 5,
		3, 7, 5,
		2, 6, 3,
		3, 6, 7,
		4, 5, 7,
		4, 7, 6,
		0, 4, 2,
		2, 4, 6,
		0, 1, 4,
		1, 5, 4,
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
	//for (int i = 0; i < 80; i++)
	//{
	//	Cube* newCube = new Cube();
	//	AddRenderObj(newCube);
	//}

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

	MSG msg = {};

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

			static int i = 0;
			while (!wnd->mMouse->IsEmpty())
			{
				const auto e = wnd->mMouse->Read();
				switch (e.GetType())
				{
					/*case Mouse::Event::Type::Leave:
						wnd->SetTitle(L"GONE");
						break;
					case Mouse::Event::Type::Move:
						{
							std::ostringstream oss;
							oss << "Mouse Position: (" << e.GetX() << ", " << e.GetY() << ")";
							std::string s = oss.str();
							std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
							std::wstring ws = converter.from_bytes(s);
							wnd->SetTitle(ws);
						}
						break;*/
				case Mouse::Event::Type::WheelUp:
					zoom += 0.2f;
					break;
				case Mouse::Event::Type::WheelDown:
					zoom -= 0.2f;
					break;
				}
			}
			/*if (wnd->mKeyboard->KeyIsPressed('A'))
			{
				MessageBoxW(nullptr, L"Something HapponNN!", L"A Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}*/
		}

		if (running)
		{
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			float deltaTime = (float)(0.000000001 * duration);
			start = end;

			fps = (int)(1.0f / deltaTime);
			/*std::ostringstream oss;
			oss << "FPS: " << fps;
			std::string str = oss.str();
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring ws = converter.from_bytes(str);
			wnd->SetTitle(ws);*/

			Update(deltaTime);
			RenderFrame();
		}
	}

	ShutDown();

	return (int)msg.wParam;
}

void App::Update(float deltaTime)
{
	angle += Math::Pi * deltaTime;

	for (auto o : renderObjects)
	{
		o->Update(deltaTime);
	}

	/*Matrix4 transform = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle) * Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 0.75f, 0.5f, 40.0f);
	transform.Transpose();

	Matrix4 transform1 = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle) * Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(4.0f, 0.0f, 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 0.75f, 0.5f, 40.0f);
	transform1.Transpose();

	Matrix4 transform2 = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle) * Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(-4.0f, 0.0f, 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 0.75f, 0.5f, 40.0f);
	transform2.Transpose();

	Matrix4 transform3 = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(-angle) * Matrix4::CreateRotationX(0.25f * -angle)
		* Matrix4::CreateTranslation(Vector3(wnd->mMouse->GetPosX() / (wnd->GetGraphics()->GetScreenWidth() /2.0f) - 1.0f, -wnd->mMouse->GetPosY() / (wnd->GetGraphics()->GetScreenHeight()/2.0f) + 1.0f, zoom + 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 9.0f / 16.0f, 1.0f, 10000.0f);
	transform3.Transpose();

	testCube->mObjConsts.modelToWorld = transform;
	testCube2->mObjConsts.modelToWorld = transform1;
	testCube3->mObjConsts.modelToWorld = transform2;*/
	Matrix4 transform = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle) * Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, zoom + 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 0.75f, 0.5f, 40.0f);
	transform.Transpose();
	testCube->mObjConsts.modelToWorld = transform;
}

void App::RenderFrame()
{
	Graphics* g = wnd->GetGraphics();

	// set render target
	g->SetBuffer(g->GetBackBuffer(), g->GetDepthStencilView());

	{
		// clear buffers
		g->ClearBuffer(0.0f, 0.0f, 0.0f);

		g->ClearDepthBuffer(g->GetDepthStencilView(), 1.0f);
	}

	hoovy->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);

	for (auto o : renderObjects)
	{
		o->Draw();
	}

	g->EndFrame();
}

void App::AddRenderObj(RenderObj* obj)
{
	renderObjects.push_back(obj);
}
