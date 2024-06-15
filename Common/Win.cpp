#include "Win.h"
#include <windowsx.h>
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
        title = Util::ConvertToWideChar(str);
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
	JS_SetPropertyStr(ctx, protoInstance, "addEventListener", JS_NewCFunction(ctx, &Win::addEventListener, "addEventListener", 2));
    regSizePos(ctx,protoInstance);
    regTimer(ctx, protoInstance);
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Win::constructor, winClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, winClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

LRESULT CALLBACK Win::RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        LPVOID pThis = pCS->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    auto obj = reinterpret_cast<Win*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (obj)
    {
        switch (msg)
        {
        case WM_NCCALCSIZE:
        {
            return false;
        }
        case WM_LBUTTONDOWN:
        {
            if (obj->captionPath.contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) {
                obj->isCaptionMouseDown = true;
                GetCursorPos(&obj->startPos);
                SetCapture(hWnd);
            }
            else {
                int i = obj->elements.size() - 1;
                if (i < 0) return true;
                for (; i >= 0; i--) {
                    auto element = Element::GetPtr(obj->elements[i]);
                    element->MouseDown();
                }
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            obj->isCaptionMouseDown = false;
            int i = obj->elements.size() - 1;
            if (i < 0) return true;
            for (; i >= 0; i--) {
                auto element = Element::GetPtr(obj->elements[i]);
                element->MouseUp();
            }
            break;
        }
        case WM_MOUSELEAVE: {
            //obj->mouseLeave();
            return true;
        }
        case WM_MOUSEMOVE:
        {
            if (obj->isCaptionMouseDown) {
                POINT point;
                GetCursorPos(&point);
                int dx = point.x - obj->startPos.x;
                int dy = point.y - obj->startPos.y;
                obj->x += dx;
                obj->y += dy;
                SetWindowPos(hWnd, nullptr, obj->x, obj->y, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
                obj->startPos = point;
            }
            else {
                auto x = (float)GET_X_LPARAM(lParam) / obj->scaleFactor;
                auto y = (float)GET_Y_LPARAM(lParam) / obj->scaleFactor;
                int i = obj->elements.size() - 1;
                if (i < 0) return true;
                for (; i >=0; i--) {
                    auto element = Element::GetPtr(obj->elements[i]);
                    element->MouseMove(x, y);
                }
            }
            break;
        }
        case WM_CLOSE: {
            //App::removeWindow(hWnd);
            break;
        }
        case WM_MOUSEWHEEL: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            //obj->mouseWheel(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
            break;
        }
        case WM_DPICHANGED:{
            UINT dpi = LOWORD(wParam);
            obj->scaleFactor = static_cast<float>(dpi) / 96.0f;
            obj->canvas->scale(obj->scaleFactor, obj->scaleFactor);
            RECT* rect = (RECT*)lParam;
            obj->x = rect->left;
            obj->y = rect->top;
            obj->w = rect->right - rect->left;
            obj->h = rect->bottom - rect->top;
            SetWindowPos(hWnd, NULL, obj->x, obj->y, obj->w, obj->h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
            DeleteDC(obj->hCompatibleDC);
            DeleteObject(obj->bottomHbitmap);
            obj->initCanvas();
            obj->paint();
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
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
            win->elements.push_back(JS_DupValue(ctx, element));
        }
    }
    else {
        win->elements.push_back(JS_DupValue(ctx, argv[0]));
    }    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}


JSValue Win::refresh(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->paint();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::addEventListener(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    //todo unbind
    const char* key = JS_ToCString(ctx, argv[0]);
    if (!key) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    if (!JS_IsFunction(ctx, argv[1])) {
        JS_FreeCString(ctx, key);
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    if (strcmp(key, "paint") == 0) {
        win->printCB.push_back(JS_DupValue(ctx, argv[1]));
    }
    else if (strcmp(key, "size") == 0) { //todo

    }
    JS_FreeCString(ctx, key);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

