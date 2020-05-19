#include "Form.h"

#pragma region Private

LRESULT CALLBACK Form::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void Form::MsgProc()
{
	MSG msg = { 0 };
	int iGetOK = 0;
	while ((iGetOK = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOK == -1)
			ExitProcess(3);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#pragma endregion


#pragma region Public

Form::Form(HINSTANCE hInst, LPSTR csCaption, int Width, int Height)
{
	hMainInst = hInst;
	WNDCLASS wcWindowClass = { 0 };

	csWndClass = new char[1024]{ "CrosswordsClass_" };

	lstrcat(csWndClass, strtok(csCaption, " "));

	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = csWndClass;
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;

	RegisterClass(&wcWindowClass);

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = (screen_rect.right / 2) - (Width / 2);
	int y = (screen_rect.left / 2) - (Height / 2);

	hWindow = CreateWindow(csWndClass, csCaption, WINDOW_NORMA_NOMAX | WS_VISIBLE, x, y, Width, Height, NULL, NULL, hInst, NULL);
}

void Form::Show()
{
	ShowWindow()
}

void Form::Hide()
{

}

#pragma endregion