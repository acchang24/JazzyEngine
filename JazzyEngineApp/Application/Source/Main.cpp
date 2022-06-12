#include "stdafx.h"
#include "Window.h"
#include "Exception.h"
#include "App.h"
#include <iomanip>
#include <codecvt>

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	bool running = true;
	float time = 0.0f;
	try
	{
		return App{}.Run();
	}
	catch (const Exception& e)
	{
		MessageBoxW(nullptr, e.What().c_str(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}