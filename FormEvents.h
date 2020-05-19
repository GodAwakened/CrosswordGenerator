#pragma once
#include "WinControls/WinUI.h"
#include <gdiplus.h>
#include <shlobj_core.h>
#pragma comment (lib,"Gdiplus.lib")

Form MainForm;
CrossRete* Rete;
HWND hStartDialog;
bool isDlgAccept;

Button* btnNew, *btnSave, *btnClear;
Label* lbl;
Edit* editWord;
Button* btnAccept;
ListBox* wordsList;

LRESULT CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //bidlo_code

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;  
	UINT  size = 0;   

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}

wchar_t CharToWchar(char c)
{
	char absC[] = { 'й', 'ц', 'у', 'к', 'е', 'н', 'г', 'ш', 'щ', 'з', 'х', 'ъ', 'ф', 'ы', 'в', 'а', 'п', 'р', 'о', 'л', 'д', 'ж', 'э', 'я', 'ч', 'с', 'м', 'и', 'т', 'ь', 'б', 'ю',
	'Й', 'Ц', 'У', 'К', 'Е', 'Н', 'Г', 'Ш', 'Щ', 'З', 'Х', 'Ъ', 'Ф', 'Ы', 'В', 'А', 'П', 'Р', 'О', 'Л', 'Д', 'Ж', 'Э', 'Я', 'Ч', 'С', 'М', 'И', 'Т', 'Ь', 'Б', 'Ю' };
	wchar_t absWC[] = { L'й', L'ц', L'у', L'к', L'е', L'н', L'г', L'ш', L'щ', L'з', L'х', L'ъ', L'ф', L'ы', L'в', L'а', L'п', L'р', L'о', L'л', L'д', L'ж', L'э', L'я', L'ч', L'с', L'м', L'и', L'т', L'ь', L'б', L'ю',
	L'Й', L'Ц', L'У', L'К', L'Е', L'Н', L'Г', L'Ш', L'Щ', L'З', L'Х', L'Ъ', L'Ф', L'Ы', L'В', L'А', L'П', L'Р', L'О', L'Л', L'Д', L'Ж', L'Э', L'Я', L'Ч', L'С', L'М', L'И', L'Т', L'Ь', L'Б', L'Ю' };

	for (int i = 0; i < 64; i++)
	{
		if (absC[i] == c)
			return absWC[i];
	}
}

void BtnSave_OnClick(Button* sender)
{
	wchar_t* path = new wchar_t[1024];
	memset(path, 0, 1024);
	wchar_t title[] = L"Выберите папку для сохранения результата";

	BROWSEINFOW browseInfo;
	browseInfo.hwndOwner = MainForm.GetHwnd();
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = path;
	browseInfo.lpszTitle = title;
	browseInfo.ulFlags = NULL;
	browseInfo.lpfn = NULL;
	browseInfo.iImage = 0;
	browseInfo.lParam = NULL;

	LPITEMIDLIST lpItem = SHBrowseForFolderW(&browseInfo);

	if (lpItem == NULL)
	{
		return;
	}

	SHGetPathFromIDListW(lpItem, path);

	using namespace Gdiplus;

	SIZE matrixSize;
	Rete->GetMatrixSize(&matrixSize);
	int Width = matrixSize.cx * 20 * 5;
	int Height = matrixSize.cy * 20 * 5;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap newBitmap(matrixSize.cx * 20 * 5, matrixSize.cy * 20 * 5);
	Graphics g(&newBitmap);

	SolidBrush whiteBrush(Color(255, 255, 255));
	Pen blackpen(Color(0, 0, 0), 6);
	g.FillRectangle(&whiteBrush, 0, 0, newBitmap.GetWidth(), newBitmap.GetHeight());

	for (int i1 = 0, i2 = 0; i1 < Height; i1 += 100, i2++)
	{
		for (int j1 = 0, j2 = 0; j1 < Width; j1 += 100, j2++)
		{
			if (Rete->matrix[i2][j2] != 0)
			{
				g.DrawRectangle(&blackpen, i1, j1, 100, 100);
			}
		}
	}

	newBitmap.RotateFlip(RotateFlipType::Rotate270FlipY);

	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, 60, FontStyleRegular, UnitPixel);
	PointF point(15.0f, 15.0f);
	SolidBrush blackBrush(Color(0, 0, 0));
	wchar_t symWC[2] = { '\0', '\0' };

	CLSID jpgClsid;
	GetEncoderClsid(L"image/jpeg", &jpgClsid);
	wchar_t buff[1024] = { 0 };
	wsprintfW(buff, L"%ws\\%ws", path, L"Чистый.jpg");
	newBitmap.Save(buff, &jpgClsid);

	for (int i1 = 0, i2 = 0; i1 < Height; i1 += 100, i2++)
	{
		for (int j1 = 0, j2 = 0; j1 < Width; j1 += 100, j2++)
		{
			if (Rete->matrix[i2][j2] != 0)
			{
				point.X = 15 + j1; point.Y = 15 + i1;
				symWC[0] = CharToWchar(Rete->matrix[i2][j2]);
				g.DrawString(symWC, 1, &font, point, &blackBrush);
			}
		}
	}

	wsprintfW(buff, L"%ws\\%ws", path, L"Заполненый.jpg");
	newBitmap.Save(buff, &jpgClsid);

	//GdiplusShutdown(gdiplusToken); //throw exception
}

void BtnClear_OnClick(Button* sender)
{
	Rete->Clear();
}

void BtnNew_OnClick(Button* sender)
{
	Rete->Clear();

	MainForm.Hide();
	HWND hWnd = CreateDialog(reinterpret_cast<HINSTANCE>(GetWindowLongPtrA(MainForm.GetHwnd(), GWLP_HINSTANCE)), MAKEINTRESOURCE(101), MainForm.GetHwnd(), (DLGPROC)DlgProc);
	ShowWindow(hWnd, SW_SHOW);
	isDlgAccept = false;

}

void BtnAccept_OnClick(Button* sender)
{
	char* buff = new char[255]{ 0 };
	editWord->GetText(buff, 255);
	Rete->SetWord(buff);
	delete[] buff;
}

void wordsList_OnDblClick(ListBox* sender)
{
	int id = sender->GetCurrentItemID();
	std::string csWord = sender->GetItem(id);
	Rete->SetWord(csWord.c_str());
}

void Form_Init()
{
	btnNew = new Button(MainForm.GetHwnd(), 60, 25, 20, 410);
	btnNew->SetText("New");
	btnNew->OnClick = BtnNew_OnClick;

	btnSave = new Button(MainForm.GetHwnd(), 60, 25, 90, 410);
	btnSave->SetText("Export");
	btnSave->OnClick = BtnSave_OnClick;

	btnClear = new Button(MainForm.GetHwnd(), 60, 25, 160, 410);
	btnClear->SetText("Clear");
	btnClear->OnClick = BtnClear_OnClick;

	btnAccept = new Button(MainForm.GetHwnd(), 120, 25, -50, -50);
	btnAccept->SetText("Принять");
	btnAccept->OnClick = BtnAccept_OnClick;

	lbl = new Label(MainForm.GetHwnd(), 20, 480);
	lbl->SetText("Выберите слово");

	wordsList = new ListBox(MainForm.GetHwnd(), 50, 480, 240, 300);
	wordsList->OnDblClick = wordsList_OnDblClick;

	editWord = new Edit(MainForm.GetHwnd(), 50, 480, 240, 25);
	Rete = CrossRete::CreateInstance(MainForm.GetHwnd());

	MainForm.AddControl(btnNew);
	MainForm.AddControl(btnSave);
	MainForm.AddControl(btnClear);
	MainForm.AddControl(lbl);
	MainForm.AddControl(editWord);
	MainForm.AddControl(btnAccept);
	MainForm.AddControl(wordsList);
}