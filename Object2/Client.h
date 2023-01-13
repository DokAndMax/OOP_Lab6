#pragma once
#include "framework.h"

class Client
{
public:
	BOOL Run(HWND);

	void OnCopyData(HWND, HWND, PCOPYDATASTRUCT);
	void OnInitDialog(HWND);
};