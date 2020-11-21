#include "xWindows.h"
#include "Window.h"

int WINAPI wWinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	PWSTR	  pCmdLine, 
	int		  nCmdShow)
{
	try
	{
		Window win(680, 420, "Ein atemberaubender Window");

		MSG msgReciever;
		BOOL msgReturn;
		while (msgReturn = GetMessage(&msgReciever, NULL, 0, 0) > 0)
		{
			//	TranslateMessage(&msgReciever);
			DispatchMessage(&msgReciever);
		}


		if (msgReturn = -1)
		{
			return -1;
		}
		return msgReciever.wParam;
	}
	catch (const xException& e)
	{
		MessageBox(NULL, e.what(), NULL, MB_ABORTRETRYIGNORE);
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, e.what(), NULL, MB_ABORTRETRYIGNORE);
	}
	catch ( ... )
	{
		MessageBox(NULL, "Unknown error message", NULL, MB_ABORTRETRYIGNORE);
	}
	return -1;
}