#pragma once
#include <Windows.h>
#include "Control.h"

class Edit : public Control
{
private:
	bool MultiLine;

	void CreateEdit(HWND hParent);
	int GenerateIDM();
public:

	void SetMultiLine(bool multiLine);
	void SetLocation(POINT* pNewLocation);
	void GetLocation(POINT* pLocation);
	void SetSize(SIZE* pNewSize);
	void GetSize(SIZE* pSize);
	void SetVisible(bool visible);
	void GetText(LPSTR csBuff, int maxLen);
	Edit(HWND hParent);
	Edit(HWND hParent, int x, int y, int Width, int Height);
	Edit(HWND hParent, POINT* pLocation, SIZE* pSize);
};