#include "CrossRete.h"

CrossRete* CrossRete::Instance = NULL;

LRESULT CALLBACK CrossRete::ReteProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN64
	LONG_PTR pCurRete = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	CrossRete* curRete = reinterpret_cast<CrossRete*>(pCurRete);
#else
	CrossRete* curRete = reinterpret_cast<CrossRete*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
#endif

	if (message == WM_PAINT)
	{
		PAINTSTRUCT ps;
		RECT rectRete;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rectRete);
		HBRUSH brushDef = CreateSolidBrush(RGB(0x22, 0x22, 0x22));
		HBRUSH brushRed = CreateSolidBrush(RGB(0xE7, 0x41, 0x41));
		HBRUSH brushWhite = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));

		FillRect(hdc, &rectRete, brushDef);
		brushDef = CreateSolidBrush(RGB(0x87, 0x88, 0x8A));
		FrameRect(hdc, &rectRete, brushDef);

		rectRete.top = 0; rectRete.left = 0;
		rectRete.bottom = 20; rectRete.right = 20;
		brushDef = CreateSolidBrush(RGB(0x88, 0x88, 0x88));

		for (int i = 0; i < curRete->matrixSize.cy; i++)
		{
			for (int j = 0; j < curRete->matrixSize.cx; j++)
			{
				if (curRete->matrix[i][j] == 1)
				{
					FillRect(hdc, &rectRete, brushRed);
				}
				else if (curRete->matrix[i][j] != 0)
				{
					FillRect(hdc, &rectRete, brushWhite);

					char* buff = new char[2]{ curRete->matrix[i][j], '\0' };
					DrawText(hdc, buff, 1, &rectRete, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					delete[] buff;
				}
				FrameRect(hdc, &rectRete, brushDef);
				
				rectRete.left += 20; rectRete.right += 20;
			}
			rectRete.left = 0; rectRete.right = 20;
			rectRete.top += 20; rectRete.bottom += 20;
		}


		EndPaint(hWnd, &ps);
	}
	else if (message == WM_LBUTTONUP)
	{
		if (curRete->curState == STATES::NORMAL || curRete->curState == STATES::BEG)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(curRete->GetHwnd(), &pt);

			SIZE curMatrixSize;
			curRete->GetMatrixSize(&curMatrixSize);

			for (int i = 0; i < curMatrixSize.cy; i++)
			{
				for (int j = 0; j < curMatrixSize.cx; j++)
				{
					if ((pt.x >= j * 20 && pt.x <= (j + 1) * 20) && (pt.y >= i * 20 && pt.y <= (i + 1) * 20))
					{
						if (curRete->curState == STATES::NORMAL)
						{
							if (curRete->matrix[i][j] == 0)
								curRete->matrix[i][j] = 1;
							curRete->startPoint = { j, i };
							curRete->curState = STATES::BEG;
						}
						else if (curRete->curState == STATES::BEG)
						{
							if ((curRete->startPoint.x == j && curRete->startPoint.y == i) ||
								(curRete->startPoint.x != j && curRete->startPoint.y != i))
							{
								MessageBeep(10);
								return 0;
							}

							curRete->endPoint = { j, i };

							if (curRete->startPoint.x == curRete->endPoint.x)
							{
								if (curRete->endPoint.y > curRete->startPoint.y)
								{
									for (int i = curRete->startPoint.y; i <= curRete->endPoint.y; i++)
										if (curRete->matrix[i][curRete->startPoint.x] == 0)
											curRete->matrix[i][curRete->startPoint.x] = 1;
								}
								else
								{
									for (int i = curRete->startPoint.y; i >= curRete->endPoint.y; i--)
										if (curRete->matrix[i][curRete->startPoint.x] == 0)
											curRete->matrix[i][curRete->startPoint.x] = 1;
								}
							}
							else if (curRete->startPoint.y == curRete->endPoint.y)
							{
								if (curRete->endPoint.x > curRete->startPoint.x)
								{
									for (int i = curRete->startPoint.x; i <= curRete->endPoint.x; i++)
										if (curRete->matrix[curRete->startPoint.y][i] == 0)
											curRete->matrix[curRete->startPoint.y][i] = 1;
								}
								else
								{
									for (int i = curRete->startPoint.x; i >= curRete->endPoint.x; i--)
										if (curRete->matrix[curRete->startPoint.y][i] == 0)
											curRete->matrix[curRete->startPoint.y][i] = 1;
								}
							}

							curRete->curState = STATES::SELECTING;
						}
					}
				}
			}

			SendMessage(curRete->Handle, WM_PAINT, 0, 0);
			InvalidateRect(curRete->Handle, NULL, FALSE);
		}

	}
	else if (message == WM_RBUTTONUP)
	{
		if (curRete->curState == STATES::SELECTING)
		{
			if (curRete->startPoint.x == curRete->endPoint.x)
			{
				if (curRete->endPoint.y > curRete->startPoint.y)
				{
					for (int i = curRete->startPoint.y; i <= curRete->endPoint.y; i++)
						if (curRete->matrix[i][curRete->startPoint.x] == 1)
							curRete->matrix[i][curRete->startPoint.x] = 0;
				}
				else
				{
					for (int i = curRete->startPoint.y; i >= curRete->endPoint.y; i--)
						if (curRete->matrix[i][curRete->startPoint.x] == 1)
							curRete->matrix[i][curRete->startPoint.x] = 0;
				}
			}
			else if (curRete->startPoint.y == curRete->endPoint.y)
			{
				if (curRete->endPoint.x > curRete->startPoint.x)
				{
					for (int i = curRete->startPoint.x; i <= curRete->endPoint.x; i++)
						if (curRete->matrix[curRete->startPoint.y][i] == 1)
							curRete->matrix[curRete->startPoint.y][i] = 0;
				}
				else
				{
					for (int i = curRete->startPoint.x; i >= curRete->endPoint.x; i--)
						if (curRete->matrix[curRete->startPoint.y][i] == 1)
							curRete->matrix[curRete->startPoint.y][i] = 0;
				}
			}
			else
			{
				MessageBox(0, "Возникла неизвестная ошибка. Обязательно сообщите мне о том, как Вы на нее наткнулись\nи я исправлю ее :)", "Error!", MB_OK | MB_ICONERROR);
				return 0;
			}

			curRete->startPoint = { 0, 0 };
			curRete->endPoint = { 0, 0 };
			curRete->curState = STATES::NORMAL;

			SendMessage(curRete->Handle, WM_PAINT, 0, 0);
			InvalidateRect(curRete->Handle, NULL, FALSE);
		}
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	return 0;
}

void CrossRete::Clear()
{
	curState = STATES::NORMAL;
	for (int i = 0; i < matrixSize.cy; i++)
	{
		for (int j = 0; j < matrixSize.cx; j++)
		{
			matrix[i][j] = 0;
		}
	}

	SendMessage(Handle, WM_PAINT, 0, 0);
	InvalidateRect(Handle, NULL, FALSE);
}

bool CrossRete::SetWord(LPCSTR word)
{
	if (curState != STATES::SELECTING)
		return false;

	int wordLen = lstrlen(word);
	int wordInReteLen;

	if (startPoint.x == endPoint.x)
	{
		int ed = endPoint.y > startPoint.y ? 1 : -1;

		wordInReteLen = abs(endPoint.y - startPoint.y) + 1;
		if (wordInReteLen != wordLen)
			return false;

		if (ed == 1)
		{
			for (int i = 0, j = startPoint.y; j <= endPoint.y; j += ed, i++)
			{
				if (matrix[j][endPoint.x] != 1 && matrix[j][endPoint.x] != word[i])
				{
					MessageBeep(10);
					return false;
				}
			}

			for (int i = 0, j = startPoint.y; j <= endPoint.y; i++, j += ed)
			{
				matrix[j][startPoint.x] = word[i];
			}
		}
		else
		{
			//for (int i = 0, j = startPoint.y; j >= endPoint.y; j += ed, i++)
			for (int i = lstrlen(word) - 1, j = startPoint.y; j >= endPoint.y; j += ed, i--)
			{
				if (matrix[j][endPoint.x] != 1 && matrix[j][endPoint.x] != word[i])
				{
					MessageBeep(10);
					return false;
				}
			}

			for (int i = lstrlen(word)-1, j = startPoint.y; j >= endPoint.y; i--, j += ed)
			{
				matrix[j][startPoint.x] = word[i];
			}
		}
	}
	else if (startPoint.y == endPoint.y)
	{
		int ed = endPoint.x > startPoint.x ? 1 : -1;

		wordInReteLen = abs(endPoint.x - startPoint.x) + 1;
		if (wordInReteLen != wordLen)
			return false;

		if (ed == 1)
		{
			for (int i = 0, j = startPoint.x; j <= endPoint.x; j += ed, i++)
			{
				if (matrix[endPoint.y][j] != 1 && matrix[endPoint.y][j] != word[i])
				{
					MessageBeep(10);
					return false;
				}
			}

			for (int i = 0, j = startPoint.x; j <= endPoint.x; i++, j += ed)
			{
				matrix[endPoint.y][j] = word[i];
			}
		}
		else
		{
			//for (int i = 0, j = startPoint.x; j >= endPoint.x; j += ed, i++)
			for (int i = lstrlen(word) - 1, j = startPoint.x; j >= endPoint.x; j += ed, i--)
			{
				if (matrix[endPoint.y][j] != 1 && matrix[endPoint.y][j] != word[i])
				{
					MessageBeep(10);
					return false;
				}
			}

			for (int i = lstrlen(word)-1, j = startPoint.x; j >= endPoint.x; i--, j += ed)
			{
				matrix[endPoint.y][j] = word[i];
			}
		}
	}
	else
		return false;

	curState = STATES::NORMAL;
	startPoint = { 0, 0 };
	endPoint = { 0, 0 };
	SendMessage(Handle, WM_PAINT, 0, 0);
	InvalidateRect(Handle, NULL, FALSE);
}

int CrossRete::GetWordSize()
{
	int len = 0;

	if (startPoint.x == endPoint.x)
		len = abs(endPoint.y - startPoint.y);
	else
		len = abs(endPoint.x - startPoint.x);

	return len;
}

bool CrossRete::GetMask(char* mask)
{
	mask[0] = '\0';

	if (curState == STATES::SELECTING)
	{
		if (startPoint.x == endPoint.x)
		{
			int j = 0;
			if (endPoint.y > startPoint.y)
			{
				for (int i = startPoint.y; i <= endPoint.y; i++, j++)
				{
					if (matrix[i][startPoint.x] == 1)
						mask[j] = '.';
					else
						mask[j] = matrix[i][startPoint.x];
				}
			}
			else if (startPoint.y > endPoint.y)
			{
				j = abs(endPoint.y - startPoint.y) + 1;
				for (int i = startPoint.y, k = j - 1; i >= endPoint.y; i--, k--)
				{
					if (matrix[i][startPoint.x] == 1)
						mask[k] = '.';
					else
						mask[k] = matrix[i][startPoint.x];
				}
			}

			mask[j] = '\0';
		}
		else if (startPoint.y == endPoint.y)
		{
			int j = 0;
			if (endPoint.x > startPoint.x)
			{
				for (int i = startPoint.x; i <= endPoint.x; i++, j++)
				{
					if (matrix[startPoint.y][i] == 1)
						mask[j] = '.';
					else
						mask[j] = matrix[startPoint.y][i];
				}
			}
			else if (startPoint.x > endPoint.x)
			{
				j = abs(endPoint.x - startPoint.x) + 1;
				for (int i = startPoint.x, k = j - 1; i >= endPoint.x; i--, k--)
				{
					if (matrix[startPoint.y][i] == 1)
						mask[k] = '.';
					else
						mask[k] = matrix[startPoint.y][i];
				}
			}

			mask[j] = '\0';
		}
		else
			return false;
	}
}

CrossRete* CrossRete::CreateInstance(HWND hParent)
{
	if (Instance != NULL)
		return Instance;

	Instance = new CrossRete(hParent);
	return Instance;
}

CrossRete::CrossRete(HWND hParent)
{
	matrix = new char* [15];
	for (int i = 0; i < 15; i++)
	{
		matrix[i] = new char[15];
		RtlSecureZeroMemory(matrix[i], 15);
	}

	matrixSize = { 15, 15 };
	startPoint = { 0, 0 }; endPoint = { 0, 0 };
	curState = STATES::NORMAL;

	Rete_IDM = 7777777;
#ifdef _WIN64
	LONG_PTR pInst = GetWindowLongPtr(hParent, GWLP_HINSTANCE);
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(pInst);
#else
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
#endif
	Handle = CreateWindow("STATIC", "", WS_CHILD | WS_BORDER | WS_VISIBLE, 5, 5, 20 * matrixSize.cx, 20 * matrixSize.cy, hParent, reinterpret_cast<HMENU>(Rete_IDM), hInst, NULL);
	SetWindowLongPtr(Handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ReteProc));
	SetWindowLongPtr(Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(Handle, SW_SHOW);
}

HWND CrossRete::GetHwnd()
{
	return Handle;
}

CrossRete* CrossRete::GetInstance()
{
	return Instance;
}

bool CrossRete::SetMatrixSize(SIZE* pNewSize)
{
	if (pNewSize->cx > 20 || pNewSize->cy > 20 || pNewSize->cx < 10 || pNewSize->cy < 10)
		return false;

	for (int i = 0; i < matrixSize.cy; i++)
		delete[] matrix[i];

	delete matrix;

	matrix = new char* [pNewSize->cy];
	for (int i = 0; i < pNewSize->cy; i++)
	{
		matrix[i] = new char[pNewSize->cx];
		RtlSecureZeroMemory(matrix[i], pNewSize->cx);
	}

	matrixSize.cx = pNewSize->cx;
	matrixSize.cy = pNewSize->cy;

	SendMessage(Handle, WM_PAINT, 0, 0);
	SetWindowPos(Handle, HWND_TOP, 0, 0, pNewSize->cx * 20, pNewSize->cy * 20, SWP_NOMOVE);
	InvalidateRect(Handle, NULL, FALSE);
	//InvalidateRect(GetParent(Handle), NULL, FALSE);

	return true;
}

void CrossRete::GetMatrixSize(SIZE* pSize)
{
	memcpy(pSize, &matrixSize, sizeof(SIZE));
}