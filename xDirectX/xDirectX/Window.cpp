#include "Window.h"

Window::WindowClass Window::WindowClass::wndCreation;

Window::WindowClass::WindowClass()
	:
	hInstance(GetModuleHandle(NULL))
{
	WNDCLASSEX wndClass = {0};

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = WindowProcInstall;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = CreateSolidBrush(0x001E1E1E);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = pClassName;

	RegisterClassEx(&wndClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(pClassName, hInstance);
}

const char * Window::WindowClass::getClassName()
{
	return pClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return wndCreation.hInstance;
}

Window::Window(int width, int height, const char * windowName)
	:
	width(width),
	height(height)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = height;
	rect.right = width;

	if (AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE) == 0)
		throw WND_LAST_ERROR_CHECK();
	
	wndHandle = CreateWindowEx(
		0, WindowClass::getClassName(),
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, WindowClass::GetInstance() , this );

	if (wndHandle == NULL)
		throw WND_LAST_ERROR_CHECK();

	ShowWindow(wndHandle, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(wndHandle);
}

LRESULT Window::WindowProcInstall(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProcRedirect));
					// I can't pWindow->WindowProc  because of if (msg == WM_NCCREATE)
					// I can't delete the if() because lParam is relevent only by this one message WN_NCCREATE
		return pWindow->WindowProc(hWnd, msg, wParam, lParam);				// from this point the class won't be calling DefWindowProc anymore
	}
																			// this shit has to return either a global function or DefWindowProc
	return DefWindowProc(hWnd, msg, wParam, lParam);						// because of the type-match ( Window:WindowProc ain't a WindowProc )
}

LRESULT Window::WindowProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->WindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYUP:
		if (wParam == 'Q')
			SetWindowTextA(hwnd, "Who else would you have tought that?");
		break;
	case WM_KEYDOWN:
		if (wParam == 'Q')
			SetWindowTextA(hwnd, "Don't wait, just do it");
		break;
	case WM_LBUTTONDOWN:
		std::ostringstream oss;
		POINT point = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
		oss << "(" << point.x << ")" << "    " << "(" << point.y << ")";
		SetWindowText(hwnd, oss.str().c_str());
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::WindowException::WindowException(int line, const char * file, HRESULT hr) noexcept
	:
	xException(line,file),
	hr(hr)
{
}

const char * Window::WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << "[FILE]: " << file << std::endl;
	oss << "[LINE]: " << line;
	oss << "[MESSAGE]: " << GetErrorString();

	msgbuffer = oss.str();
	return msgbuffer.c_str();
}

std::string Window::WindowException::GetErrorMsg(HRESULT hr) noexcept
{
	char* pMsgBuffer = nullptr;
	DWORD pMsgBufferLenght = FormatMessage (
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer),
		0, NULL );
	if (pMsgBufferLenght == 0) {
		return "the fuck is this error code??? undefined";
	}

	std::string error = pMsgBuffer;
	LocalFree(pMsgBuffer);
	return error;
}

std::string Window::WindowException::GetErrorString() const noexcept
{
	return GetErrorMsg(hr);
}
