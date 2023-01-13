#include "Plot.h"
#include <algorithm>

#define SQUARES 15
#define RADIUS 5

void Plot::ProcessData(void* data, long size, std::function<void(void*, long)>)
{
    double* ab = (double*)data;
    int arrSize = size / sizeof(double);
    std::vector<double>arr((double*)data, (double*)data + arrSize);
    points = arr;
}

void Plot::DrawPlot(HWND hWnd, HDC hdc)
{
    GetClientRect(hWnd, &wnd);

    sqSize = max(wnd.right, wnd.bottom) / SQUARES;
    sqWidth = (int)floor((double)wnd.right / sqSize) - 3;
    sqHeight = (int)floor((double)wnd.bottom / sqSize) - 3;


    DrawLine(hdc, { 0, 0 }, { 0, sqHeight });
    DrawLine(hdc, { 0, 0 }, { sqWidth, 0 });

    for (int i = 0; i <= sqWidth; i++)
    {
        SetPos(hdc, i, -0.3);
        DrawLine(hdc, i, 0.3);
    }

    for (int i = 0; i <= sqHeight; i++)
    {
        SetPos(hdc, -0.3, i);
        DrawLine(hdc, 0.3, i);
    }

    DrawPoints(hdc);
}

void Plot::DrawPoints(HDC hdc)
{
    WCHAR buffer[64];
    double xMax = 0;
    double xMin = 0;
    double yMax = 0;
    double yMin = 0;
    double xInterval = 0;
    double yInterval = 0;
    if (points.size())
    {
        xMax  = (double)points.size() - 1;
        yMax = *std::max_element(points.begin(), points.end());
        yMin = *std::min_element(points.begin(), points.end());
        xInterval = (double)(xMax - xMin) / sqWidth;
        yInterval = (double)(yMax - yMin) / sqHeight;
    }

    for (int i = 0; i <= sqWidth; i++)
    {
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
            L"%g", (int)(xMin + xInterval * i * 100.0) / 100.0);
        DrawTextToPlot(hdc, i, -0.8, buffer);
    }
    for (int i = 0; i <= sqHeight; i++)
    {
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
            L"%g", (int)((yMin + yInterval * i) * 100.0) / 100.0);
        DrawTextToPlot(hdc, -0.8,  i, buffer);
    }

    for (int i = 0; i < points.size(); i++)
    {
        if (i == 0)
            SetPos(hdc, (i - xMin) / xInterval, ((double)points[i] - yMin) / yInterval);
        else
            DrawLine(hdc, (i - xMin) / xInterval, ((double)points[i] - yMin) / yInterval);
    }

    xInterval = xInterval ? xInterval : 1;
    yInterval = yInterval ? yInterval : 1;
    for (int i = 0; i < points.size(); i++)
        DrawEllipse(hdc, (i - xMin) / xInterval, ((double)points[i] - yMin) / yInterval);

}

POINT Plot::GetClientCoords(double x, double y)
{
    return POINT{ static_cast<LONG>(sqSize * (x + 1.5)),
        static_cast<LONG>(wnd.bottom - (sqSize * (y + 1.5))) };
}

void Plot::SetPos(HDC hdc, double x, double y)
{
    POINT coords = GetClientCoords(x, y);
    MoveToEx(hdc, coords.x, coords.y, NULL);
}

void Plot::DrawLine(HDC hdc, double x, double y)
{
    POINT coords = GetClientCoords(x, y);
    LineTo(hdc, coords.x, coords.y);
}

void Plot::DrawLine(HDC hdc, POINT from, POINT to)
{
    SetPos(hdc, from.x, from.y);
    DrawLine(hdc, to.x, to.y);
}

void Plot::DrawEllipse(HDC hdc, double x, double y)
{
    POINT coords = GetClientCoords(x, y);
    Ellipse(hdc, coords.x - RADIUS, coords.y - RADIUS,
        coords.x + RADIUS, coords.y + RADIUS);
}

void Plot::DrawTextToPlot(HDC hdc, double posx, double posy, LPCWSTR text)
{
    SetTextAlign(hdc, TA_BASELINE | TA_CENTER | TA_NOUPDATECP);
    POINT coords = GetClientCoords(posx, posy);
    TextOut(hdc, coords.x, coords.y, text, (int)wcslen(text));
}