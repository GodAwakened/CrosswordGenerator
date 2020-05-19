#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include "Control.h"
#include <vector>
#include <string>

class ListBox : public Control
{
private:
	typedef void(_cdecl* OnClick_prt)(ListBox* sender);
	std::vector<std::string> Items;

	int GenerateIDM();
	void CreateControl(HWND hParent);

public:

	ListBox(HWND hParent);
	~ListBox();
	ListBox(HWND hParent, int x, int y, int Width, int Height);
	ListBox(HWND hParent, POINT* pLocation, SIZE* pSize);
	void GetSize(SIZE* pSize);
	void GetLocation(POINT* pLocation);
	void SetSize(SIZE* pNewSize);
	void SetLocation(POINT* pNewLocation);
	std::string GetItem(int index);
	void AddItem(LPCSTR item);
	void DelItem(int index);
	int GetItemsCount() { return Items.size(); };
	void SetVisible(bool visible);
	bool GetVisible();
	void Clear();
	int GetCurrentItemID();

	OnClick_prt OnDblClick;
};