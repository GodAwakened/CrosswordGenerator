#pragma once
#include <Windows.h>
#include "Control.h"

class Label : public Control
{
private:
	void CreateLabel(HWND hParent);
	void GetSizeForText(HWND hParent, SIZE* pSize);
	int GenerateIDM();
public:
	
	void GetText(LPSTR csBuff);
	void SetText(LPCSTR csText);
	void GetLocation(POINT* pLocation);
	void SetLocation(POINT* pNewLocation);
	void SetVisible(bool visible);
	Label(HWND hParent);
	Label(HWND hParent, int x, int y);
	Label(HWND hParent, POINT* pLocation);
	~Label();
};