#pragma once
#include "framework.h"
class Client
{
public:
	BOOL Run(HWND);

	void OnCopyData(HWND, HWND, PCOPYDATASTRUCT);
	BOOL OnClipboardUpdate(HWND);
	BOOL OnDestroy(HWND);
	BOOL OnPaint(HWND, HDC);
};