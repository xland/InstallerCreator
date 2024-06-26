#include "Util.h"
#include "Windows.h"
#include <format>
#ifdef DEBUG
#include <iostream>
#include <fcntl.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
void Util::InitDebuger() {
    //AttachConsole(ATTACH_PARENT_PROCESS) == TRUE;
    //AllocConsole();
    //auto std_handle = (intptr_t)GetStdHandle(STD_OUTPUT_HANDLE);
    //auto con_handle = _open_osfhandle(std_handle, _O_TEXT);
    //auto fp = _fdopen(con_handle, "w");
    //freopen_s(&fp, "CONOUT$", "w", stdout);
    //std_handle = (intptr_t)GetStdHandle(STD_ERROR_HANDLE);
    //con_handle = _open_osfhandle(std_handle, _O_TEXT);
    //fp = _fdopen(con_handle, "w");
    //freopen_s(&fp, "CONOUT$", "w", stderr);
    if (!::AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent()) {
        if (::AllocConsole()) {
            FILE* unused;
            if (freopen_s(&unused, "CONOUT$", "w", stdout)) {
                _dup2(_fileno(stdout), 1);
            }
            if (freopen_s(&unused, "CONOUT$", "w", stderr)) {
                _dup2(_fileno(stdout), 2);
            }
            std::ios::sync_with_stdio();
        }
    }
}
#endif
std::wstring Util::ConvertToWStr(const char* str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], count);
    if (wstr.length() == 1 && str[0] == L'\0') {
        wstr = std::wstring{};
    }
    return wstr;
}
std::string Util::ConvertToStr(const std::wstring& wstr)
{
    const int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}
void Util::CheckEmptyString(std::wstring& str)
{
    if(str.length() == 1 && str[0] == L'\0') {
        str = std::wstring{};
        bool flag = str.empty();
    }
}
GUID Util::ConvertToGuid(const char* str) {
    auto wGuidStr = ConvertToWStr(str);
    wGuidStr = std::format(L"{{{}}}", wGuidStr);
    GUID guid;
    HRESULT hr = CLSIDFromString(wGuidStr.data(), &guid);
    return guid;
}
