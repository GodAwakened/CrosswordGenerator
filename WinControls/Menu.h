#pragma once
#include <Windows.h>
#include <vector>

class Menu
{
private:
	char* Caption;

	int GenerateIDM();
public:
	int IDM_MENU;
	HMENU Handle;

	Menu(LPCSTR caption);
	~Menu();
	void AppendItem(LPCSTR caption);
	void GetCaption(LPSTR csBuff);
	void AppendToPopup(Menu* item);
};