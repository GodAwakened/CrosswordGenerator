#include "WinControls/WinUI.h"
#include "FormEvents.h"
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "resource.h"

//#define USING_LAMBDA_IN_LIST

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
			if (MainForm.GetHwnd() == hWnd)
			{
				if (MainForm.OnFormClick != NULL)
					MainForm.OnFormClick(&MainForm);
			}
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
			case BN_CLICKED:
				for (Control* curControl : MainForm.AllControls)
				{
					if (curControl->GetIDM() == LOWORD(wParam))
					{
						Button* curButton = reinterpret_cast<Button*>(curControl);

						if (curButton->OnClick != NULL)
							curButton->OnClick(curButton);
					}
				}
				break;
			case LBN_DBLCLK:
				for (Control* curControl : MainForm.AllControls)
				{
					if (curControl->GetIDM() == LOWORD(wParam))
					{
						ListBox* curButton = reinterpret_cast<ListBox*>(curControl);

						if (curButton->OnDblClick != NULL)
							curButton->OnDblClick(curButton);
					}
				}
				break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (LOWORD(wParam) == startDlg_acceptBtn)
				{
					int x, y;
					char* csTemp = new char[255]{ 0 };
					GetWindowText(GetDlgItem(hWnd, startDlg_xEdit), csTemp, 255);

					if (!strcmp(csTemp, ""))
					{
						MessageBox(NULL, "Заполните все поля!", "Ошибка!", MB_OK | MB_ICONERROR);
						return 0;
					}

					x = atoi(csTemp);
					GetWindowText(GetDlgItem(hWnd, startDlg_xEdit), csTemp, 255);

					if (!strcmp(csTemp, ""))
					{
						MessageBox(NULL, "Заполните все поля!", "Ошибка!", MB_OK | MB_ICONERROR);
						return 0;
					}

					y = atoi(csTemp);
					delete[] csTemp;

					if (x > 20 || y > 20)
					{
						MessageBeep(10);
						MessageBox(NULL, "Максимальный размер сетки 20x20", "Ошибка!", MB_OK | MB_ICONERROR);
					}
					else if (x < 10 || y < 10)
					{
						MessageBeep(10);
						MessageBox(NULL, "Минимальный размер сетки 10x10", "Ошибка!", MB_OK | MB_ICONERROR);
					}
					else
					{
						SIZE matrixSize = { x, y };
						Rete->SetMatrixSize(&matrixSize);
						matrixSize.cx = matrixSize.cx * 20 + 350;
						matrixSize.cy = matrixSize.cy * 20 + 100;
						MainForm.SetSize(&matrixSize);

						MainForm.Show();
						isDlgAccept = true;
						DestroyWindow(hWnd);

						SIZE curSize;
						btnNew->GetSize(&curSize);
						POINT point = { 20, matrixSize.cy - curSize.cy - 50 };
						btnNew->SetLocation(&point);

						btnSave->GetSize(&curSize);
						point = { 90, matrixSize.cy - curSize.cy - 50 };
						btnSave->SetLocation(&point);

						btnClear->GetSize(&curSize);
						point = { 160, matrixSize.cy - curSize.cy - 50 };
						btnClear->SetLocation(&point);

						point = { matrixSize.cx - 240, 10 };
						lbl->SetLocation(&point);
						lbl->SetVisible(false);

						point = { matrixSize.cx - 300, 40 };
						editWord->SetLocation(&point);
						editWord->SetVisible(false);

						point = { matrixSize.cx - 250, 80 };
						btnAccept->SetLocation(&point);
						btnAccept->SetVisible(false);

						point = { matrixSize.cx - 300, 125 };
						int cy = (matrixSize.cy - 100) * 0.6f;
						wordsList->GetSize(&curSize);
						curSize = { curSize.cx, cy };
						wordsList->SetLocation(&point);
						wordsList->SetSize(&curSize);
						wordsList->SetVisible(false);
					}
				}
				else if (LOWORD(wParam) == startDlg_cancelBtn)
				{
					PostQuitMessage(0);
				}
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			if (!isDlgAccept)
				PostQuitMessage(0);
			break;
	}

	return 0;
}

DWORD WINAPI ThreadProc(LPVOID params)
{
	using namespace std;
	vector<string> words;

	ifstream file;
	file.open("words.txt", fstream::in);
	string line;
	while (getline(file, line))
		words.push_back(line);

	bool flagVisible = false;

	int offsets[21] = { 0, 0, WORDS_POS_2, WORDS_POS_3, WORDS_POS_4, WORDS_POS_5,
	WORDS_POS_6, WORDS_POS_7, WORDS_POS_8, WORDS_POS_9, WORDS_POS_10, WORDS_POS_11,
	WORDS_POS_12, WORDS_POS_13, WORDS_POS_14, WORDS_POS_15, WORDS_POS_16, WORDS_POS_17,
	WORDS_POS_18, WORDS_POS_19, WORDS_POS_20 };

	char buffMask[21];
	memset(buffMask, 0, 21);

	while (true)
	{
		if (Rete->isSelecting())
		{
			if (flagVisible)
				continue;

			flagVisible = true;
			lbl->SetVisible(flagVisible);
			btnAccept->SetVisible(flagVisible);
			editWord->SetVisible(flagVisible);
			wordsList->SetVisible(flagVisible);
			
			wordsList->Clear();
			Rete->GetMask(buffMask);
			regex regMask(buffMask);

			/*for (string str : words)	//578 ticks
			{
				if (regex_match(str.c_str(), regMask))
				{
					wordsList->AddItem(str.c_str());
				}
			}*/

			int masklen = lstrlen(buffMask);
			int offset = offsets[masklen];
			int endOffset = 0;
			if (masklen != 20)
				endOffset = offsets[masklen + 1];
#ifndef USING_LAMBDA_IN_LIST
			else
				endOffset = words.size();

			for (int i = offset; i < endOffset; i++) //62 ticks
			{
				string str = words[i];
				if (regex_match(str.c_str(), regMask))
					wordsList->AddItem(str.c_str());
			}
#else
			vector<string>::iterator it = words.begin();
			vector<string>::iterator endIt = endOffset == 0 ? words.end() : words.begin() + endOffset;

			it = find_if(it, endIt, [regMask](const string& str) -> bool {
				return regex_match(str.c_str(), regMask);
				});

			while (it != endIt) //500 ticks
			{
				wordsList->AddItem(it->c_str());
				it++;

				it = find_if(it, endIt, [regMask](const string& str) -> bool { 
						return regex_match(str.c_str(), regMask); 
					});
			}
#endif
		}
		else if (flagVisible)
		{
			flagVisible = false;
			lbl->SetVisible(flagVisible);
			btnAccept->SetVisible(flagVisible);
			editWord->SetVisible(flagVisible);
			wordsList->SetVisible(flagVisible);
		}

		Sleep(100);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR csCmdLine, int cmdCode)
{
	//Form_Init(hInstance, reinterpret_cast<WNDPROC>(WndProc));
	MainForm = Form(hInstance, "CrosswordGenerator", 800, 550, reinterpret_cast<WNDPROC>(WndProc));
	//MainForm.Show();

	Form_Init();
	
	ShowWindow(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), MainForm.GetHwnd(), (DLGPROC)DlgProc), SW_SHOW);
	isDlgAccept = false;

	CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);

	MSG msg = { 0 };
	int iGetOK = 0;
	while ((iGetOK = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOK == -1)
			return 3;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}