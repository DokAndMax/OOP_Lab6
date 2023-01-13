#pragma once
#include "framework.h"
#include "IDataHandler.h"
#include <functional>
#include <map>

class Interaction
{
private:
	std::map<int, std::function<void(void*, int)>> cbfuncs;
	std::map<int, IDataHandler*> msgcbfuncs;

	BOOL SendDataUID(HWND, HWND, int, int, void*, long, std::function<void(void*, int)>);
	BOOL ReplyToData(HWND, HWND, int, int, void*, long);
	BOOL ProcessReply(int, void*, int);
public:

	BOOL SendData(HWND, HWND, int, void*, long, std::function<void(void*, int)>);
	BOOL ProcessData(HWND, HWND, PCOPYDATASTRUCT);
	void SetMessageCallback(int, IDataHandler*);
};