#include "App.h"
#include <Windows.h>
#include "include/core/SkGraphics.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/ports/SkTypeface_win.h"


namespace App {

    sk_sp<SkFontMgr> fontMgr;
    std::shared_ptr<SkFont> fontIcon{ nullptr };
    std::shared_ptr<SkFont> fontText{ nullptr };

	void Init()
	{
        SkGraphics::Init();
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr)) return;
        fontMgr = SkFontMgr_New_GDI();
        fontText = std::make_shared<SkFont>(fontMgr->matchFamilyStyle("Microsoft YaHei", {}));
        initFontIcon();
        return;
	}

    void Dispose()
    {
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