#include "Client.h"
#include "Interaction.h"
#include "MyTable.h"
#include <commctrl.h>

static Interaction intr;
static MyTable lvTable;

BOOL Client::Run(HWND hWnd)
{
    intr.SetMessageCallback(1, &lvTable);

	return TRUE;
}

void Client::OnCopyData(HWND hWndDest, HWND hWndSrc, PCOPYDATASTRUCT copyData)
{
	intr.ProcessData(hWndDest, hWndSrc, copyData);
}

void Client::OnInitDialog(HWND hWnd)
{
    lvTable.OnInitDialog(hWnd);
}