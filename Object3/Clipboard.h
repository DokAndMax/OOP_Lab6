#pragma once
#include "framework.h"
#include "IDataHandler.h"
#include <map>
class Clipboard
{
private:
	std::map<UINT, IDataHandler*> msgcbfuncs;
	void* GetFromClipboard(UINT, long*);
public:
	template<class T>
	BOOL PutToClipboard(T, long);
	template<class T>
	T GetFromClipboard(long*);

	BOOL ProcessClipboardUpdate(HWND);
	template<class T>
	void SetClipboardUpdateCallback(IDataHandler*);
};

template<class T>
BOOL Clipboard::PutToClipboard(T data, long size)
{
	LPCSTR formatName = typeid(data).name();
	UINT format = RegisterClipboardFormatA(formatName);

	HGLOBAL hglbCopy;
	if (&data == NULL || size == 0) return FALSE;
	hglbCopy = GlobalAlloc(GHND, size);
	if (hglbCopy == NULL) return FALSE;
	LPVOID pTmp = GlobalLock(hglbCopy);
	if (std::is_pointer<T>::value)
		memcpy_s(pTmp, size, data, size);
	else
		memcpy_s(pTmp, size, &data, size);

	GlobalUnlock(hglbCopy);
	if (!OpenClipboard(NULL))
	{
		GlobalFree(hglbCopy);
		return TRUE;
	}
	EmptyClipboard();
	SetClipboardData(format, hglbCopy);
	CloseClipboard();
	return TRUE;
}

template<class T>
T Clipboard::GetFromClipboard(long* size)
{
	LPCSTR formatName = typeid(T).name();
	UINT format = RegisterClipboardFormatA(formatName);

	T data = *(T*)GetFromClipboard(format, size);

	return data;
}

template<class T>
void Clipboard::SetClipboardUpdateCallback(IDataHandler* msg)
{
	LPCSTR formatName = typeid(T).name();
	UINT format = RegisterClipboardFormatA(formatName);

	msgcbfuncs.insert(std::pair<UINT, IDataHandler*>(format, msg));
}