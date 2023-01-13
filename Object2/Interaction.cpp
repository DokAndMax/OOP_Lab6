#include "interaction.h"

static int _uId = 0;

BOOL Interaction::SendDataUID(HWND hWndDest, HWND hWndSrc, int mId,
    int uId, void* lp, long cb, std::function<void(void*, int)> func)
{
    COPYDATASTRUCT cds{};
    cds.dwData = MAKELONG(mId, uId);
    cds.cbData = cb;
    cds.lpData = lp;
    if (func)
        cbfuncs.insert(std::pair<int, std::function<void(void*, int)>>(uId, func));

    return (BOOL)SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}

BOOL Interaction::SendData(HWND hWndDest, HWND hWndSrc, int mId,
    void* lp, long cb, std::function<void(void*, int)> func)
{
    return SendDataUID(hWndDest, hWndSrc, mId, _uId++, lp, cb, func);
}

BOOL Interaction::ProcessData(HWND hWndDest, HWND hWndSrc, PCOPYDATASTRUCT data)
{
    int mId = LOWORD(data->dwData);
    int uId = HIWORD(data->dwData);
    void* mData = data->lpData;
    long size = data->cbData;
    if (mId <= 0)
        return ProcessReply(uId, mData, data->cbData);
    else if (mId > 0)
        return ReplyToData(hWndDest, hWndSrc, mId, uId, mData, size);
    return FALSE;
}

BOOL Interaction::ReplyToData(HWND hWndDest, HWND hWndSrc, int mId, int uId, void* data, long size)
{
    void* resData = nullptr;
    auto itr = msgcbfuncs.find(mId);
    BOOL res = FALSE;
    if (itr != msgcbfuncs.end())
        itr->second->ProcessData(data, size, [this, hWndDest, hWndSrc, uId, &res](void* d, int dsize)
            {
                res = SendDataUID(hWndDest, hWndSrc, 0, uId, d, dsize, NULL);
            });
    if (res)
        SendDataUID(hWndDest, hWndSrc, -1, uId, nullptr, NULL, NULL);
    return res;
}

BOOL Interaction::ProcessReply(int uId, void* data, int size)
{
    auto itr = cbfuncs.find(uId);
    if (itr != cbfuncs.end())
    {
        itr->second(data, size);
        cbfuncs.erase(itr);
    }
    return TRUE;
}

void Interaction::SetMessageCallback(int mId, IDataHandler* msg)
{
    msgcbfuncs.insert(std::pair<int, IDataHandler*>(mId, msg));
}
