#pragma once
#include "framework.h"
#include "IDataHandler.h"
#include <vector>

class MyTable : public IDataHandler
{
private:
	HWND lvhWnd;
public:
	struct DataParameters;
	void ProcessData(void*, long, std::function<void(void*, long)>);

	void FillLV(std::vector<double>);
	void OnInitDialog(HWND);
};