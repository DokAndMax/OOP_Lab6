#pragma once
#include <functional>
class IDataHandler
{
public:
	virtual void ProcessData(void*, long, std::function<void(void*, long)>) = 0;
};