#include "Menu.h"

int Menu::GenerateIDM()
{
	static int idm = 0;
	idm++;

	return idm;
}

void Menu::GetCaption(LPSTR buff)
{
	lstrcpy(buff, Caption);
}

void Menu::AppendItem(LPCSTR caption)
{
	AppendMenu(Handle, MF_STRING, GenerateIDM(), caption);
}

Menu::Menu(LPCSTR caption)
{
	IDM_MENU = GenerateIDM();
	Handle = CreateMenu();
	Caption = new char[lstrlen(caption) + 1];
	lstrcpy(Caption, caption);
}

Menu::~Menu()
{
	delete[] Caption;
}

void Menu::AppendToPopup(Menu* item)
{
	AppendMenu(Handle, MF_POPUP, (UINT_PTR)item->Handle, Caption);
}