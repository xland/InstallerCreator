#include "Win.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <map>
#include <string>
#include <format>
#include "JS.h"
#include "Util.h"
#include "Paint.h"
#include "Path.h"

namespace {
    static float scaleFactor;
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
            win->w = w*scaleFactor;
        }
        if (JS_ToInt32(ctx, &h, argv[2])) {
            return JS_ThrowTypeError(ctx, "arg2 error");
        }
        else {
            win->h = h * scaleFactor;
        }
        if (JS_ToInt32(ctx, &x, argv[3])) {
            return JS_ThrowTypeError(ctx, "arg3 error");
        }
        else {
            win->x = x * scaleFactor;
        }
        if (JS_ToInt32(ctx, &y, argv[4])) {
            return JS_ThrowTypeError(ctx, "arg4 error");
        }
        else {
            win->y = y * scaleFactor;
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
            win->w = w * scaleFactor;
        }
        if (JS_ToInt32(ctx, &h, argv[2])) {
            return JS_ThrowTypeError(ctx, "arg2 error");
        }
        else {
            win->h = h * scaleFactor;
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
    HDC hScreenDC = GetDC(NULL);
    int systemDpiX = GetDeviceCaps(hScreenDC, LOGPIXELSX);
    scaleFactor = (static_cast<float>(systemDpiX) / 96.0f);
    ReleaseDC(NULL, hScreenDC);


	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &winClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "setPos", JS_NewCFunction(ctx, &Win::setPos, "setPos", 2));
    JS_SetPropertyStr(ctx, protoInstance, "getPos", JS_NewCFunction(ctx, &Win::getPos, "getPos", 0));
    JS_SetPropertyStr(ctx, protoInstance, "setSize", JS_NewCFunction(ctx, &Win::setSize, "setSize", 2));
    JS_SetPropertyStr(ctx, protoInstance, "getSize", JS_NewCFunction(ctx, &Win::getSize, "getSize", 0));

    JS_SetPropertyStr(ctx, protoInstance, "setCaptionPath", JS_NewCFunction(ctx, &Win::setCaptionPath, "setCaptionPath", 0));
	JS_SetPropertyStr(ctx, protoInstance, "setPosCenterScreen", JS_NewCFunction(ctx, &Win::setPosCenterScreen, "setPosCenterScreen", 0));
	JS_SetPropertyStr(ctx, protoInstance, "refresh", JS_NewCFunction(ctx, &Win::refresh, "refresh", 0));
	JS_SetPropertyStr(ctx, protoInstance, "show", JS_NewCFunction(ctx, show, "show", 0));
	JS_SetPropertyStr(ctx, protoInstance, "addEventListener", JS_NewCFunction(ctx, addEventListener, "addEventListener", 2));

    JS_SetPropertyStr(ctx, protoInstance, "fillColor", JS_NewCFunction(ctx, &Win::fillColor, "fillColor", 1));
    JS_SetPropertyStr(ctx, protoInstance, "drawRect", JS_NewCFunction(ctx, &Win::drawRect, "drawRect", 5));
    JS_SetPropertyStr(ctx, protoInstance, "drawEllipse", JS_NewCFunction(ctx, &Win::drawEllipse, "drawEllipse", 5));
    JS_SetPropertyStr(ctx, protoInstance, "drawShadow", JS_NewCFunction(ctx, &Win::drawShadow, "drawShadow", 3));

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
            auto path = Path::getPtr(obj->captionPath);
            if (path->contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) {
                obj->isCaptionMouseDown = true;
                GetCursorPos(&obj->startPos);
                SetCapture(hWnd);
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            obj->isCaptionMouseDown = false;
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
            break;
        }
        case WM_CLOSE: {
            //App::removeWindow(hWnd);
            return true;
        }
        case WM_MOUSEWHEEL: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            //obj->mouseWheel(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
            break;
        }
        case WM_TIMER: {
            //obj->onTimeout(wParam);
            break;
        }
        case WM_DPICHANGED:{
            UINT dpi = LOWORD(wParam);
            scaleFactor = static_cast<float>(dpi) / 96.0f;
            obj->canvas->scale(scaleFactor, scaleFactor);
            RECT* rect = (RECT*)lParam;
            obj->x = rect->left;
            obj->y = rect->top;
            obj->w = rect->right - rect->left;
            obj->h = rect->bottom - rect->top;
            SetWindowPos(hWnd, NULL, obj->x, obj->y, obj->w, obj->h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
            DeleteDC(obj->hCompatibleDC);
            DeleteObject(obj->bottomHbitmap);
            obj->initCanvas();
            obj->paintWindow();
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
    paintWindow();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
}

void Win::paintWindow()
{
    canvas->clear(0x00000000);
    for (size_t i = 0; i < printCB.size(); i++)
    {
        auto ctx = JS::GetCtx();
        JSValue ret = JS_Call(ctx, printCB[i], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
        JS_FreeValue(ctx, ret);
    }
    HDC hdc = GetDC(hwnd);
    BITMAPINFO info = { sizeof(BITMAPINFOHEADER), w, 0 - h, 1, 32, BI_RGB, w * 4 * h, 0, 0, 0, 0 };
    SetDIBits(hdc, bottomHbitmap, 0, h, surfaceMemory.get(), &info, DIB_RGB_COLORS);
    BLENDFUNCTION blend = { .BlendOp{AC_SRC_OVER}, .SourceConstantAlpha{255}, .AlphaFormat{AC_SRC_ALPHA} };
    POINT pSrc = { 0, 0 };
    SIZE sizeWnd = { w, h };
    UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);
    ReleaseDC(hwnd, hdc);
    //surfaceMemory.reset(0); //实践证明这样即节省内存，速度也不会慢
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

JSValue Win::setPos(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    int x, y;
    if (JS_ToInt32(ctx, &x, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    else {
        win->x = x;
    }
    if (JS_ToInt32(ctx, &y, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    else {
        win->y = y;
    }
    SetWindowPos(win->hwnd, NULL, win->x, win->y, win->w, win->h, SWP_NOZORDER | SWP_NOSIZE);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::getPos(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ret, "x", JS_NewInt32(ctx, win->x));
    JS_SetPropertyStr(ctx, ret, "y", JS_NewInt32(ctx, win->y));
    return ret;
}

JSValue Win::setSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    int w, h;
    if (JS_ToInt32(ctx, &w, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    else {
        win->w = w;
    }
    if (JS_ToInt32(ctx, &h, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    else {
        win->h = h;
    }
    SetWindowPos(win->hwnd, NULL, win->x, win->y, win->w, win->h, SWP_NOZORDER);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::getSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ret, "w", JS_NewInt32(ctx, win->w));
    JS_SetPropertyStr(ctx, ret, "h", JS_NewInt32(ctx, win->h));
    return ret;
}

JSValue Win::setCaptionPath(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->captionPath = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::setPosCenterScreen(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    RECT screenRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    win->x = (screenRect.right - win->w) / 2;
    win->y = (screenRect.bottom - win->h) / 2;
    SetWindowPos(win->hwnd, NULL, win->x, win->y, win->w, win->h, SWP_NOZORDER | SWP_NOSIZE);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::refresh(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->paintWindow();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::addEventListener(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* key = JS_ToCString(ctx, argv[0]);
    if (!key) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    if (!JS_IsFunction(ctx, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->printCB.push_back(JS_DupValue(ctx, argv[1]));
    JS_FreeCString(ctx, key);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
