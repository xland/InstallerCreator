#include "Win.h"
#include <dwmapi.h>
#include <map>
#include <string>
#include <format>
#include "JS.h"
#include "Util.h"
#include "Path.h"
#include "Element.h"

namespace {
    static JSClassID id;
	static JSClassDef winClass = {
		.class_name{"Win"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
                auto ctx = JS::GetCtx();
                auto win = (Win*)JS_GetOpaque(val, id);
                delete win;
            }
        }
	};
}
Win::Win()
{

}
Win::~Win()
{
    DeleteDC(hCompatibleDC);
    DeleteObject(bottomHbitmap);
}

Win* Win::getPtr(JSValue& val)
{
    auto win = (Win*)JS_GetOpaque(val, id);
    return win;
}

JSValue Win::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
    Win* win = new Win();
    HDC hScreenDC = GetDC(NULL);
    int systemDpiX = GetDeviceCaps(hScreenDC, LOGPIXELSX);
    win->scaleFactor = (static_cast<float>(systemDpiX) / 96.0f);
    ReleaseDC(NULL, hScreenDC);
    std::wstring title;
    const char* str = JS_ToCString(ctx, argv[0]);
    if (str) {
        title = Util::ConvertToWStr(str);
        JS_FreeCString(ctx, str);
    }
    if (argc == 5) {
        int x, y, w, h;
        if (JS_ToInt32(ctx, &w, argv[1])) {
            return JS_ThrowTypeError(ctx, "arg1 error");
        }
        else {
            win->w = w* win->scaleFactor;
        }
        if (JS_ToInt32(ctx, &h, argv[2])) {
            return JS_ThrowTypeError(ctx, "arg2 error");
        }
        else {
            win->h = h * win->scaleFactor;
        }
        if (JS_ToInt32(ctx, &x, argv[3])) {
            return JS_ThrowTypeError(ctx, "arg3 error");
        }
        else {
            win->x = x * win->scaleFactor;
        }
        if (JS_ToInt32(ctx, &y, argv[4])) {
            return JS_ThrowTypeError(ctx, "arg4 error");
        }
        else {
            win->y = y * win->scaleFactor;
        }
        win->initWindow(title);
        win->initCanvas();
        JS_SetOpaque(obj, win);
        return obj;
    }
    if (argc == 3) {
        int w, h;
        if (JS_ToInt32(ctx, &w, argv[1])) {
            return JS_ThrowTypeError(ctx, "arg1 error");
        }
        else {
            win->w = w * win->scaleFactor;
        }
        if (JS_ToInt32(ctx, &h, argv[2])) {
            return JS_ThrowTypeError(ctx, "arg2 error");
        }
        else {
            win->h = h * win->scaleFactor;
        }
    }
    RECT screenRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    win->x = (screenRect.right - win->w) / 2;
    win->y = (screenRect.bottom - win->h) / 2;
    win->initWindow(title);
    win->initCanvas();
	JS_SetOpaque(obj, win);
	return obj;
}

void Win::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &winClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "refresh", JS_NewCFunction(ctx, &Win::refresh, "refresh", 0));
	JS_SetPropertyStr(ctx, protoInstance, "show", JS_NewCFunction(ctx, &Win::show, "show", 0));
    JS_SetPropertyStr(ctx, protoInstance, "minimize", JS_NewCFunction(ctx, &Win::minimize, "minimize", 0));
    JS_SetPropertyStr(ctx, protoInstance, "close", JS_NewCFunction(ctx, &Win::close, "close", 0));
    JS_SetPropertyStr(ctx, protoInstance, "addElement", JS_NewCFunction(ctx, &Win::addElement, "addElement", 1));
    JS_SetPropertyStr(ctx, protoInstance, "removeElement", JS_NewCFunction(ctx, &Win::removeElement, "removeElement", 1));
    regSizePos(ctx,protoInstance);
    regTimer(ctx, protoInstance);
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Win::constructor, winClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, winClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

void Win::initWindow(std::wstring& title)
{
	static int num = 0;
	std::wstring className = std::format(L"Installer{}", num++);
	auto hinstance = GetModuleHandle(NULL);
	WNDCLASSEX wcx{};
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcx.lpfnWndProc = &Win::RouteWindowMessage;
	wcx.cbWndExtra = sizeof(Win*);
	wcx.hInstance = hinstance;
	wcx.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(hinstance, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszClassName = className.c_str();
	if (!RegisterClassEx(&wcx)) return;
    auto style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
	this->hwnd = CreateWindowEx(WS_EX_LAYERED, className.c_str(), title.c_str(), style, x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this)); 
}

void Win::show()
{
    paint();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
}

void Win::paint()
{
    canvas->clear(0x00000000);
    for (size_t i = 0; i < elements.size(); i++) {
        auto element = Element::GetPtr(elements[i]);
        element->Paint(this);
    }
    HDC hdc = GetDC(hwnd);
    BITMAPINFO info = { sizeof(BITMAPINFOHEADER), w, 0 - h, 1, 32, BI_RGB, w * 4 * h, 0, 0, 0, 0 };
    SetDIBits(hdc, bottomHbitmap, 0, h, surfaceMemory.get(), &info, DIB_RGB_COLORS);
    BLENDFUNCTION blend = { .BlendOp{AC_SRC_OVER}, .SourceConstantAlpha{255}, .AlphaFormat{AC_SRC_ALPHA} };
    POINT pSrc = { 0, 0 };
    SIZE sizeWnd = { w, h };
    UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);
    ReleaseDC(hwnd, hdc);
}

void Win::closed()
{
    printf("  %s\n", "closed");
    auto ctx = JS::GetCtx();
    for (size_t i = 0; i < elements.size(); i++)
    {
        auto ele = Element::GetPtr(elements[i]);
        ele->Dispose();
        JS_FreeValue(ctx, elements[i]);
    }
    elements.clear();
    PostQuitMessage(0);
}

void Win::initCanvas()
{
    HDC hdc = GetDC(hwnd);
    hCompatibleDC = CreateCompatibleDC(NULL);
    bottomHbitmap = CreateCompatibleBitmap(hdc, w, h);
    DeleteObject(SelectObject(hCompatibleDC, bottomHbitmap));
    ReleaseDC(hwnd, hdc);

    surfaceMemory.reset(w * h * 4);
    SkImageInfo info = SkImageInfo::MakeN32Premul(w, h);
    auto temp = SkCanvas::MakeRasterDirect(info, surfaceMemory.get(), 4 * w);
    canvas = std::move(temp);
    UINT dpi = GetDpiForWindow(hwnd);
    auto scaleFactor = (static_cast<float>(dpi) / 96.0f);
    canvas->scale(scaleFactor, scaleFactor);
}

JSValue Win::show(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->show();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::minimize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    ShowWindow(win->hwnd, SW_MINIMIZE);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::close(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->DisposeTimer();
    PostMessage(win->hwnd, WM_CLOSE, 0, 0);    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::addElement(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    if (JS_IsArray(ctx,argv[0])) {
        uint32_t length = 0;
        JS_ToUint32(ctx, &length, JS_GetPropertyStr(ctx, argv[0], "length"));
        for (uint32_t i = 0; i < length; ++i) {
            JSValue element = JS_GetPropertyUint32(ctx, argv[0], i);
            win->elements.push_back(element);
        }
    }
    else {
        win->elements.push_back(argv[0]);
    }    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::removeElement(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    const char* strData = JS_ToCString(ctx, argv[0]);
    if (!strData) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    std::string idStr(strData);
    std::erase_if(win->elements, [&idStr,&ctx](auto& item) { 
        auto element = Element::GetPtr(item);
        if (!element->idStr.empty() && element->idStr == idStr) {
            element->Dispose();
            JS_FreeValue(ctx, item);
            return true;
        }
        return false;
    });
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}


JSValue Win::refresh(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->paint();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}