#pragma once

class Control
{
protected:
	HWND Handle;
	int IDM;
	char* Text;
	int x, y;
	int Width, Height;
	bool Visible;

	virtual int GenerateIDM() = 0;
public:
	virtual HWND GetHwnd() { return Handle; }
	virtual int GetIDM() { return IDM; }
	virtual void GetSize(SIZE* pSize) { return; } //Reserved
	virtual void GetLocation(POINT* pLocation) { return; } //Reserved
	virtual void SetSize(SIZE* pNewSize) { return; } //Reserved
	virtual void SetLocation(POINT* pNewLocation) { return; } //Reserved
};