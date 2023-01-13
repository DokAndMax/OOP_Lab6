#pragma once
#include "framework.h"
#include "string"

class Client
{
private:
	HWND hWndObj2;
	HWND hWndObj3;

	HWND StartModule(HWND, std::string, std::wstring);

	int ParseIntFromControl(HWND, int);
public:
	struct DataParameters;
	BOOL Run(HWND);

	void OnCopyData(HWND, HWND, PCOPYDATASTRUCT);
	BOOL OnDestroy(HWND);
};