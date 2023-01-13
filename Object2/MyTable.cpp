#include "MyTable.h"
#include "resource.h"
#include "Clipboard.h"
#include <commctrl.h>

struct MyTable::DataParameters {
    int n;
    int min;
    int max;
};

void MyTable::ProcessData(void* arrNums, long size, std::function<void(void*, long)> func)
{
    DataParameters params = *(DataParameters*)arrNums;
    int n = params.n;
    int min = params.min;
    int max = params.max;

    std::vector<double> data;

    for (int i = 0; i < n; i++)
        data.push_back(((double)max - (double)min) * ((double)rand() / (double)RAND_MAX) + min);
   
    Clipboard cp;
    cp.PutToClipboard<double*>(data.data(), (long)data.size() * sizeof(data[0]));
    func(data.data(), (long)data.size() * sizeof(data[0]));

    FillLV(data);
}

void MyTable::FillLV(std::vector<double> arr)
{
    if (!lvhWnd)
        return;

    ListView_DeleteAllItems(lvhWnd);
    WCHAR buffer[64];
    LVITEM lvI{};
    lvI.mask = LVIF_TEXT;
    for (int i = 0; i < arr.size(); i++)
    {
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", i);
        lvI.iItem = ListView_GetItemCount(lvhWnd);
        lvI.pszText = buffer;
        if (ListView_InsertItem(lvhWnd, &lvI) == -1)
            return;
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%lf", arr[i]);
        ListView_SetItemText(lvhWnd, lvI.iItem, 1, buffer);
    }
}

void MyTable::OnInitDialog(HWND hWnd)
{
    lvhWnd = GetDlgItem(hWnd, IDC_LIST1);
    ListView_SetExtendedListViewStyle(lvhWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
        LVS_EX_AUTOSIZECOLUMNS);

    LPWSTR columnText[]{
        (LPWSTR)L"x",
        (LPWSTR)L"y",
    };
    INT columnCount = sizeof(columnText) / sizeof(columnText[0]);

    RECT rect;
    INT width = 0;
    if (GetClientRect(lvhWnd, &rect))
        width = rect.right - rect.left;

    LVCOLUMN lvc{};
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH | LVCFMT_FIXED_RATIO;
    lvc.cx = width / columnCount;

    // Add the columns.
    for (int i = 0; i < columnCount; i++)
    {
        lvc.pszText = columnText[i];

        ListView_InsertColumn(lvhWnd, i + 1, &lvc);

        if (i < (width % columnCount))
            ListView_SetColumnWidth(lvhWnd, i, lvc.cx + 1);
    }
}