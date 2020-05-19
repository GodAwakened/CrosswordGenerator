#include "Form.h"

#pragma region Private



#pragma endregion


#pragma region Public

Form::Form(HINSTANCE hInst, LPCSTR csCaption, int Width, int Height, WNDPROC wndproc)
{
	hMainInst = hInst;
	OnFormClick = {};
	WNDCLASS wcWindowClass = { 0 };

	char* csTempCaption = new char[lstrlen(csCaption) + 1];
	lstrcpy(csTempCaption, csCaption);

	csWndClass = new char[1024]{ "CrosswordsClass_" };

	char* csNextContext = nullptr;
	char* csCurContext = strtok_s(csTempCaption, " ", &csNextContext);

	lstrcat(csWndClass, csCurContext);

	wcWindowClass.lpfnWndProc = wndproc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = csWndClass;
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClass(&wcWindowClass);

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = (screen_rect.right / 2) - (Width / 2);
	int y = (screen_rect.bottom / 2) - (Height / 2);

	Size.cx = x; Size.cy = y;

	hWindow = CreateWindow(csWndClass, csCaption, WINDOW_NORMA_NOMAX | WS_VISIBLE, x, y, Width, Height, NULL, NULL, hInst, NULL);
	ShowWindow(hWindow, SW_HIDE);
}

void Form::SetSize(SIZE* pNewSize)
{
	memcpy(&Size, pNewSize, sizeof(SIZE));
	SetWindowPos(hWindow, HWND_TOP, 0, 0, Size.cx, Size.cy, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void Form::Show()
{
	ShowWindow(hWindow, SW_SHOW);
}

void Form::Hide()
{
	ShowWindow(hWindow, SW_HIDE);
}

Form::~Form()
{
	//delete[] csWndClass;
}

void Form::SetHeaderMenu(Menu* menu)
{
	SetMenu(hWindow, menu->Handle);
}

void Form::DeleteHeaderMenu()
{
	SetMenu(hWindow, NULL);
}

void Form::SetFormIcon(HICON hIcon)
{
	SendMessage(hWindow, WM_SETICON, ICON_BIG, (long)hIcon);
}

void Form::AddControl(Control* control)
{
	SetParent(control->GetHwnd(), hWindow);
	ShowWindow(control->GetHwnd(), SW_SHOW);
	AllControls.push_back(control);
}

void Form::DeleteFormIcon()
{

}

#pragma endregion