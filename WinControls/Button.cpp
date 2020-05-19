#include "Button.h"

#pragma region Private

#pragma endregion


#pragma region Public

void Button::GetSize(SIZE* pSize)
{
	pSize->cx = Width;
	pSize->cy = Height;
}

void Button::SetSize(SIZE* pNewSize)
{
	Width = pNewSize->cx;
	Height = pNewSize->cy;

	SetWindowPos(Handle, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void Button::GetLocation(POINT* pPoint)
{
	pPoint->x = x;
	pPoint->y = y;
}

void Button::SetLocation(POINT* pNewPoint)
{
	x = pNewPoint->x;
	y = pNewPoint->y;

	SetWindowPos(Handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
}

void Button::GetText(LPSTR csBuffer)
{
	lstrcpy(csBuffer, Text);
}

void Button::SetText(LPCSTR csNewText)
{
	delete[] Text;
	Text = new char[lstrlen(csNewText) + 1];
	lstrcpy(Text, csNewText);
	SetWindowText(Handle, Text);
}

void Button::SetVisible(bool visible)
{
	Visible = visible;
	if (Visible)
		ShowWindow(Handle, SW_SHOW);
	else
		ShowWindow(Handle, SW_HIDE);
}

Button::Button(HWND hParent)
{
	x = 0; y = 0;
	Text = new char[8] { "Button" };

	Width = 200; Height = 120;

	CreateButtonWindow(hParent);
}

Button::Button(HWND hParent, int width, int height, int x, int y)
{
	this->x = x; this->y = y;
	Text = new char[8]{ "Button" };

	Width = width; Height = height;

	CreateButtonWindow(hParent);
}

Button::Button(HWND hParent, POINT* pLocation, SIZE* pSize)
{
	x = pLocation->x; y = pLocation->y;
	Text = new char[8]{ "Button" };

	Width = pSize->cx; Height = pSize->cy;

	CreateButtonWindow(hParent);
}

int Button::GenerateIDM()
{
	static int s_idm = 1;
	IDM = s_idm++;

	return IDM;
}

void Button::CreateButtonWindow(HWND hParent)
{
	GenerateIDM();
#ifdef _WIN64
	LONG_PTR pInst = GetWindowLongPtr(hParent, GWLP_HINSTANCE);
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(pInst);
#else
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
#endif
	Handle = CreateWindow("BUTTON", Text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, x, y, Width, Height, hParent, (HMENU)IDM, hInst, NULL);
	ShowWindow(Handle, SW_SHOW);
}

Button::~Button()
{
	delete[] Text;
}

#pragma endregion
