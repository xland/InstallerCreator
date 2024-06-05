#include "Util.h"

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
std::wstring Util::ConvertToWideChar(const char* str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], count);
    return wstr;
}