#pragma once
extern "C" {
#include <quickjs.h>
}
#include <string>
#include <Windows.h>
#include <objbase.h>

class Util
{
public:
#ifdef DEBUG
	static void InitDebuger();
#endif
	static std::wstring ConvertToWStr(const char* str);
	static std::string ConvertToStr(const std::wstring& wstr);
    static void CheckEmptyString(std::wstring& str);
	static GUID ConvertToGuid(const char* str);


};

