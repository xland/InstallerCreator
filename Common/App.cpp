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

    void App::initFontIcon()
    {
        //HMODULE instance = GetModuleHandle(NULL);
        //HRSRC resID = FindResource(instance, MAKEINTRESOURCE(IDR_ICON_FONT), L"ICON_FONT");
        //if (resID == 0)
        //{
        //    MessageBox(NULL, L"查找字体图标资源失败", L"系统提示", NULL);
        //    return;
        //}
        //size_t resSize = SizeofResource(instance, resID);
        //HGLOBAL res = LoadResource(instance, resID);
        //if (res == 0)
        //{
        //    MessageBox(NULL, L"加载字体图标资源失败", L"系统提示", NULL);
        //    return;
        //}
        //LPVOID resData = LockResource(res);
        //auto fontData = SkData::MakeWithoutCopy(resData, resSize);
        //fontIcon = std::make_shared<SkFont>(fontMgr->makeFromData(fontData));
    }


}