#include "GeneratePlotHandler.h"

GeneratePlotHandler::GeneratePlotHandler(void func(double*, int))
{
	cb = func;
}

void GeneratePlotHandler::ProcessData(void* data, std::function<void(void*, int)> func)
{
	func(nullptr, NULL);
}
