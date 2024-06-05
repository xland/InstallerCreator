#pragma once
extern "C" {
#include <quickjs.h>
}
#include <string>
#include <Windows.h>

class Util
{
public:
#ifdef DEBUG
	static void InitDebuger();
#endif
	static std::wstring ConvertToWideChar(const char* str);
};

