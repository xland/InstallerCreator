#include "App.h"
#include <Windows.h>
#include "include/core/SkGraphics.h"


namespace App {
	void Init()
	{
        SkGraphics::Init();
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr)) return;
        return;
	}

    void Dispose()
    {
        CoUninitialize();
    }
}