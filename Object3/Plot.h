#pragma once
#include "IDataHandler.h"
#include "framework.h"
#include <vector>

class Plot : public IDataHandler
{
private:
	RECT wnd;
	int sqSize;
	int sqWidth;
	int sqHeight;
	std::vector<double>points;

	void DrawPoints(HDC);
	POINT GetClientCoords(double, double);
	void SetPos(HDC, double, double);
	void DrawLine(HDC, double, double);
	void DrawLine(HDC, POINT, POINT);
	void DrawEllipse(HDC, double, double);
	void DrawTextToPlot(HDC, double, double, LPCWSTR);
public:
	void DrawPlot(HWND, HDC);
	void ProcessData(void*, long, std::function<void(void*, long)>);
};