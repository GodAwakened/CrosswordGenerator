#pragma once
#include <Windows.h>
#define GetReteSizeByMatrix(matSize, bufSize) bufSize.cx = matSize.cx*20; bufSize.cy = matSize.cy*20;

#pragma region Offsets

#define		WORDS_POS_2			0
#define		WORDS_POS_3			12
#define		WORDS_POS_4			236
#define		WORDS_POS_5			985
#define		WORDS_POS_6			2304
#define		WORDS_POS_7			3465
#define		WORDS_POS_8			4503
#define		WORDS_POS_9			5301
#define		WORDS_POS_10		5882
#define		WORDS_POS_11		6197
#define		WORDS_POS_12		6222
#define		WORDS_POS_13		6247
#define		WORDS_POS_14		11658
#define		WORDS_POS_15		15387
#define		WORDS_POS_16		17618
#define		WORDS_POS_17		19000
#define		WORDS_POS_18		19825
#define		WORDS_POS_19		20293
#define		WORDS_POS_20		20551

#pragma endregion

class CrossRete //bidlo_class
{
private:
	enum STATES { NORMAL, BEG, SELECTING };

	HWND Handle;
	SIZE matrixSize;
	static CrossRete* Instance;

	POINT startPoint, endPoint;

	CrossRete(HWND hParent);
	static LRESULT CALLBACK ReteProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	int Rete_IDM;
	STATES curState;
	char** matrix;

	static CrossRete* GetInstance();
	static CrossRete* CreateInstance(HWND hParent);
	void Clear();
	bool SetWord(LPCSTR word);
	int GetWordSize();
	bool SetMatrixSize(SIZE* pNewSize);
	void GetMatrixSize(SIZE* pSize);
	HWND GetHwnd();
	bool isSelecting() { return curState == STATES::SELECTING ? true : false; }

	//the length of buff must be greater than or equal to 21
	bool GetMask(char* mask);
};