#include "Clipboard.h"

void* Clipboard::GetFromClipboard(UINT format, long* size)
{
	void* data = nullptr;

	if (!IsClipboardFormatAvailable(format)) return FALSE;
	if (!OpenClipboard(NULL)) return FALSE;
	HGLOBAL hGlb = GetClipboardData(format);
	*size = (long)GlobalSize(hGlb);
	if (hGlb != NULL)
	{
		void* buffer = (void*)GlobalLock(hGlb);

		if (buffer != NULL)
		{
			data = buffer;
			GlobalUnlock(hGlb);
		}
	}
	CloseClipboard();
	return data;
}

BOOL Clipboard::ProcessClipboardUpdate(HWND hWnd)
{
	BOOL res = FALSE;
	for (auto& item : msgcbfuncs)
	{
		if (IsClipboardFormatAvailable(item.first))
		{
			long size = 0;
			void* data = GetFromClipboard(item.first, &size);
			item.second->ProcessData(data, size, NULL);
			res = TRUE;
		}
	}
	return res;
}