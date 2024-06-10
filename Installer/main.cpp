#include <Windows.h>
#include "App.h"
#include "JS.h"
#include "Util.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
#if DEBUG
    Util::InitDebuger();
#endif
    JS::Init();
    App::Init();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    App::Dispose();
    return 0;
}