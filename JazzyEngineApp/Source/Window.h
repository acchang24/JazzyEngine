#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>

// Class to represent a window and encapsulates the creation, destruction, and message handling for a window
class Window
{
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::wstring& title);

	Graphics* GetGraphics() { return mGraphics; }

	HWND GetHwnd() { return hWnd; }

	Keyboard* mKeyboard;
	Mouse* mMouse;

private:
	// Singleton that manages the registration and cleanup of window class
	class WindowClass
	{
	public:
		static const wchar_t* GetName();
		static HINSTANCE GetInstance();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

	// Static handler to setup pointer to instance of window
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// Static handler to change from win32 api call to the HandleMsg function
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int mWidth;
	int mHeight;
	HWND hWnd;
	Graphics* mGraphics;
};
