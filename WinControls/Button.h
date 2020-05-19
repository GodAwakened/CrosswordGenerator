#pragma once
#include <Windows.h>
#include "Control.h"

class Button : public Control
{
private:
	typedef void(_cdecl* OnClick_prt)(Button* sender);

	void CreateButtonWindow(HWND hParent);
	int GenerateIDM();

public:
	void GetSize(SIZE* pSize);
	void SetSize(SIZE* pNewSize);
	void GetLocation(POINT* pPoint);
	void SetLocation(POINT* pNewPoint);
	void GetText(LPSTR csBuffer);
	void SetText(LPCSTR csNewText);
	void SetVisible(bool visible);

	~Button();
	Button(HWND hParent);
	Button(HWND hParent, int width, int height, int x, int y);
	Button(HWND hParent, POINT* pLocation, SIZE* pSize);

	OnClick_prt OnClick;
	
	friend class Form;
};

