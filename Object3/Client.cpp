#include "Client.h"
#include "Clipboard.h"
#include "Plot.h"
#include <vector>

static Clipboard clipboard;
static Plot plot;

BOOL Client::Run(HWND hWnd) 
{
    AddClipboardFormatListener(hWnd);
    clipboard.SetClipboardUpdateCallback<double*>(&plot);
    return TRUE;
}

BOOL Client::OnClipboardUpdate(HWND hWnd)
{
    if(clipboard.ProcessClipboardUpdate(hWnd))
        InvalidateRect(hWnd, NULL, TRUE);
    return TRUE;
}

BOOL Client::OnDestroy(HWND hWnd)
{
    RemoveClipboardFormatListener(hWnd);
    return TRUE;
}

BOOL Client::OnPaint(HWND hWnd, HDC hdc)
{
    plot.DrawPlot(hWnd, hdc);
    return TRUE;
}