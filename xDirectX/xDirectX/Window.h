#pragma once
#include "xWindows.h"
#include "xException.h"
#include <sstream>

class Window {
public:
	class WindowException : public xException
	{
	public:
		WindowException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		static std::string GetErrorMsg(HRESULT hr) noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
public:
	class WindowClass {													// Singleton pattern
	public:
		WindowClass();													// Constructor that gets called by a static (global) variable inside Window.cpp
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass(const WindowClass&) = delete;
		~WindowClass();
		static const char* getClassName();
		static HINSTANCE GetInstance();
	private:
		static constexpr const char* pClassName = "fuck OpenGL I guess";// constexpr - avaiable at the compile time
		static WindowClass wndCreation;									// Singleton - that's the instance of itself that gets called in the global space
		HINSTANCE hInstance;											// we might want more instances of the Window Class
	};
public:
	Window(int width, int height, const char* windowName);
	Window& operator=(const Window& win) = delete;
	Window(const Window& win) = delete;
	~Window();
	static LRESULT CALLBACK WindowProcInstall(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	int width;
	int height;
	HWND wndHandle;
};


#define WND_ERROR_CHECK(hr)					Window::WindowException(__LINE__, __FILE__, hr);
#define WND_LAST_ERROR_CHECK()				Window::WindowException(__LINE__, __FILE__, GetLastError());