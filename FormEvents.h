#pragma once
#include "WinControls/WinUI.h"
#include <gdiplus.h>
#include <shlobj_core.h>
#pragma comment (lib,"Gdiplus.lib")



LRESULT CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //bidlo_code

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void BtnSave_OnClick(Button* sender);

void BtnClear_OnClick(Button* sender);

void BtnNew_OnClick(Button* sender);

void BtnAccept_OnClick(Button* sender);

void wordsList_OnDblClick(ListBox* sender);

void Form_Init();