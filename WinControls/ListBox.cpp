#include "ListBox.h"

int ListBox::GenerateIDM()
{
	static int idm = 19001;
	IDM = idm++;

	return IDM;
}

void ListBox::GetSize(SIZE* pSize)
{
	pSize->cx = Width;
	pSize->cy = Height;
}

void ListBox::GetLocation(POINT* pLocation)
{
	pLocation->x = x;
	pLocation->y = y;
}

void ListBox::SetSize(SIZE* pNewSize)
{
	Width = pNewSize->cx;
	Height = pNewSize->cy;

	SetWindowPos(Handle, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
}

void ListBox::SetLocation(POINT* pNewLocation)
{
	x = pNewLocation->x;
	y = pNewLocation->y;

	SetWindowPos(Handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

std::string ListBox::GetItem(int index)
{
	std::string str = Items[index];
	return str.c_str();
}

void ListBox::AddItem(LPCSTR item)
{
	std::string str(item);
	//lstrcpy(buff, item);
	Items.push_back(str);

	ListBox_AddString(Handle, str.c_str());
}

void ListBox::DelItem(int index)
{
	//reserved
}

ListBox::ListBox(HWND hParent)
{
	x = 30; y = 30;
	Width = 200;
	Height = 350;
	Visible = true;

	CreateControl(hParent);
}

ListBox::ListBox(HWND hParent, int x, int y, int Width, int Height)
{
	this->x = x; this->y = y;
	this->Width = Width;
	this->Height = Height;
	Visible = true;

	CreateControl(hParent);
}

ListBox::ListBox(HWND hParent, POINT* pLocation, SIZE* pSize)
{
	x = pLocation->x; y = pLocation->y;
	Width = pSize->cx;
	Height = pSize->cy;
	Visible = true;

	CreateControl(hParent);
}

ListBox::~ListBox()
{
	/*for (int i = 0; i < Items.size(); i++)
	{
		delete[] Items.back();
	}*/
}

void ListBox::CreateControl(HWND hParent)
{
	Handle = CreateWindow("listbox", "", WS_CHILD | WS_VISIBLE | LBS_STANDARD, x, y, Width, Height, hParent, reinterpret_cast<HMENU>(GenerateIDM()),
		reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWL_HINSTANCE)), NULL);

	ShowWindow(Handle, SW_SHOW);
}

void ListBox::Clear()
{
	/*for (int i = 0; i < Items.size(); i++)
	{
		delete[] Items.back();
	}*/

	Items.clear();
	ListBox_ResetContent(Handle);
}

int ListBox::GetCurrentItemID()
{
	return ListBox_GetCurSel(Handle);
}

void ListBox::SetVisible(bool visible)
{
	Visible = visible;
	if (Visible)
		ShowWindow(Handle, SW_SHOW);
	else
		ShowWindow(Handle, SW_HIDE);
}

bool ListBox::GetVisible()
{
	return Visible;
}