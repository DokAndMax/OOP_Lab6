#include "client.h"
#include "Interaction.h"
#include "resource.h"
#include <vector>

static Interaction intr;

struct Client::DataParameters {
    int n;
    int min;
    int max;
};

HWND Client::StartModule(HWND hWnd, std::string execName, std::wstring className)
{
    std::string cmdLine;

    cmdLine = execName + " " + std::to_string((INT_PTR)hWnd);

    HWND modhWnd = FindWindow(className.c_str(), NULL);
    if (!modhWnd)
    {
        if (WinExec(cmdLine.c_str(), SW_SHOWNOACTIVATE) <= 31)
            return nullptr;
        while (!modhWnd)
            modhWnd = FindWindow(className.c_str(), NULL);
    }
    return modhWnd;
}

int Client::ParseIntFromControl(HWND hWnd, int nIDDlgItem)
{
    WCHAR num[1024];
    GetWindowText(GetDlgItem(hWnd, nIDDlgItem), num, 1024);
    return wcstol(num, NULL, 10);
}

BOOL Client::Run(HWND hWnd)
{
    hWndObj2 = StartModule(hWnd, std::string("Object2.exe"), std::wstring(L"Object2"));
    hWndObj3 = StartModule(hWnd, std::string("Object3.exe"), std::wstring(L"Object3"));

    RECT desktop;
    RECT wnd;
    RECT obj1;
    RECT obj2;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    GetWindowRect(hWnd, &wnd);
    GetWindowRect(hWndObj2, &obj1);
    GetWindowRect(hWndObj3, &obj2);

    POINT obj2pos{};
    POINT obj3pos{};
    POINT obj3size{};
    if (desktop.right - wnd.right > wnd.left)
    {
        obj2pos.x = wnd.right - (obj1.right - obj1.left);
        obj2pos.y = wnd.bottom;
        obj3pos.x = wnd.right;
        obj3pos.y = wnd.top;
        obj3size.x = desktop.right - wnd.right;
        obj3size.y = (obj2.bottom - obj2.top);
    }
    else
    {
        obj2pos.x = wnd.left;
        obj2pos.y = wnd.bottom;
        obj3pos.x = desktop.left;
        obj3pos.y = wnd.top;
        obj3size.x = wnd.left;
        obj3size.y = (obj2.bottom - obj2.top);
       
    }

    SetWindowPos(hWndObj2, HWND_TOP, obj2pos.x, obj2pos.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
    SetWindowPos(hWndObj3, HWND_TOP, obj3pos.x, obj3pos.y, obj3size.x, obj3size.y, SWP_NOACTIVATE);

    int n = ParseIntFromControl(hWnd, IDC_EDIT1);
    int min = ParseIntFromControl(hWnd, IDC_EDIT2);
    int max = ParseIntFromControl(hWnd, IDC_EDIT3);

    DataParameters params = { n, min, max};

    intr.SendData(hWndObj2, hWnd, 1, &params, sizeof(params), NULL);

    return TRUE;
}

void Client::OnCopyData(HWND hWndDest, HWND hWndSrc, PCOPYDATASTRUCT copyData)
{
    intr.ProcessData(hWndDest, hWndSrc, copyData);
}

BOOL Client::OnDestroy(HWND hWnd)
{
    if (hWndObj2)
        SendMessage(hWndObj2, WM_CLOSE, NULL, NULL);
    if (hWndObj3)
        SendMessage(hWndObj3, WM_CLOSE, NULL, NULL);
    return TRUE;
}