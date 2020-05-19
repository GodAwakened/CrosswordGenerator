#include "Edit.h"

Edit::Edit(HWND hParent)
{
	x = 10; y = 10;
	Width = 250; Height = 150;
	CreateEdit(hParent);
}

Edit::Edit(HWND hParent, int x, int y, int Width, int Height)
{
	this->x = x; this->y = y;
	this->Width = Width;
	this->Height = Height;
	CreateEdit(hParent);
}

Edit::Edit(HWND hParent, POINT* pLocation, SIZE* pSize)
{
	x = pLocation->x;
	y = pLocation->y;
	Width = pSize->cx;
	Height = pSize->cy;
	CreateEdit(hParent);
}

void Edit::SetVisible(bool visible)
{
	Visible = visible;
	if (Visible)
		ShowWindow(Handle, SW_SHOW);
	else
		ShowWindow(Handle, SW_HIDE);
}

void Edit::SetLocation(POINT* pNewLocation)
{
	x = pNewLocation->x;
	y = pNewLocation->y;

	SetWindowPos(Handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Edit::GetLocation(POINT* pLocation)
{
	pLocation->x = x;
	pLocation->y = y;
}

void Edit::SetSize(SIZE* pNewSize)
{
	Width = pNewSize->cx;
	Height = pNewSize->cy;

	SetWindowPos(Handle, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
}

void Edit::GetSize(SIZE* pSize)
{
	pSize->cx = Width;
	pSize->cy = Height;
}

void Edit::SetMultiLine(bool multiLine)
{
	MultiLine = multiLine;
}

void Edit::CreateEdit(HWND hParent)
{
	MultiLine = false;
	IDM = GenerateIDM();
	Handle = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, x, y, Width, Height, hParent, reinterpret_cast<HMENU>(IDM), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE)), NULL);
	ShowWindow(Handle, SW_HIDE);
}

void Edit::GetText(LPSTR csBuff, int maxLen)
{
	GetWindowText(Handle, csBuff, maxLen);
}

int Edit::GenerateIDM()
{
	static int idm = 130001;
	return idm++;
}