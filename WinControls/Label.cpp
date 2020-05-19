#include "Label.h"

Label::Label(HWND hParent)
{
	Text = new char[1024]{ 0 };
	lstrcpy(Text, "Label");

	SIZE labelSize;
	GetSizeForText(hParent, &labelSize);

	x = 10; y = 10;
	Width = labelSize.cx;
	Height = labelSize.cy;

	CreateLabel(hParent);
}

Label::Label(HWND hParent, int x, int y)
{
	Text = new char[1024]{ 0 };
	lstrcpy(Text, "Label");

	SIZE labelSize;
	GetSizeForText(hParent, &labelSize);

	this->x = x; this->y = y;
	Width = labelSize.cx;
	Height = labelSize.cy;

	CreateLabel(hParent);
}

Label::Label(HWND hParent, POINT* pLocation)
{
	Text = new char[1024]{ 0 };
	lstrcpy(Text, "Label");

	SIZE labelSize;
	GetSizeForText(hParent, &labelSize);

	x = pLocation->x; y = pLocation->y;
	Width = labelSize.cx;
	Height = labelSize.cy;

	CreateLabel(hParent);
}

void Label::SetVisible(bool visible)
{
	Visible = visible;
	if (Visible)
		ShowWindow(Handle, SW_SHOW);
	else
		ShowWindow(Handle, SW_HIDE);
}

void Label::GetSizeForText(HWND hParent, SIZE* pSize)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hParent, &ps);
	GetTextExtentPoint32(hdc, Text, lstrlen(Text), pSize);
	EndPaint(hParent, &ps);
}

int Label::GenerateIDM()
{
	static int idm = 3001;
	return idm++;
}

void Label::CreateLabel(HWND hParent)
{
	IDM = GenerateIDM();
#ifdef _WIN64
	LONG_PTR pInst = GetWindowLongPtr(hParent, GWLP_HINSTANCE);
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(pInst);
#else
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
#endif
	Handle = CreateWindow("STATIC", Text, WS_CHILD | WS_VISIBLE, x, y, Width, Height, hParent, (HMENU)IDM, hInst, NULL);
	ShowWindow(Handle, SW_SHOW);
}

void Label::SetText(LPCSTR csText)
{
	lstrcpy(Text, csText);
	SIZE labelSize;
	GetSizeForText(GetParent(Handle), &labelSize);
	SetWindowPos(Handle, NULL, 0, 0, labelSize.cx, labelSize.cy, SWP_NOMOVE | SWP_NOZORDER);
	SetWindowText(Handle, Text);
}

void Label::GetText(LPSTR csBuff)
{
	lstrcpy(csBuff, Text);
}

void Label::GetLocation(POINT* pLocation)
{
	pLocation->x = x;
	pLocation->y = y;
}


void Label::SetLocation(POINT* pNewLocation)
{
	x = pNewLocation->x;
	y = pNewLocation->y;

	SetWindowPos(Handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

Label::~Label()
{
	delete[] Text;
}