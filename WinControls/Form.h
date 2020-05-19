#pragma once
#include "Menu.h"
#include "Control.h"
#include <Windows.h>

#define WINDOW_NORMA_NOMAX WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX

class Form
{
private:
	typedef void(_cdecl* OnFormClick_prt)(Form* sender);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	char* csWndClass;
	HINSTANCE hMainInst;
	HWND hWindow;
	SIZE Size;
public:
	OnFormClick_prt OnFormClick;
	std::vector<Control*> AllControls; //переместить в private

	Form() { }
	~Form();
	Form(HINSTANCE hInst, LPCSTR csCaption, int Width, int Height, WNDPROC wndproc);
	void Show();
	void Hide();
	void SetHeaderMenu(Menu* menu);
	void DeleteHeaderMenu();
	void SetFormIcon(HICON hIcon);
	void DeleteFormIcon();
	void SetSize(SIZE* pNewSize);
	void AddControl(Control* control);
	HWND GetHwnd() { return hWindow; };
};