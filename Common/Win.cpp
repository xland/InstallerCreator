#include "Win.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <map>
#include <string>
#include <format>
#include "JS.h"
#include "Util.h"

namespace {
	static JSClassID id{ 0 };
	static JSClassDef jsWinClass = {
		.class_name{"Win"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto win = (Win*)JS_GetOpaque(val, id);
				delete win;
			} 
		}
	};
}
JSValue Win::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
    Win* win = new Win();
    std::wstring title;
    if (argc == 1) {
        const char* str = JS_ToCString(ctx, argv[0]);
        title = Util::ConvertToWideChar(str);
        JS_FreeCString(ctx, str);
        win->initWindow(title);
        win->centerScreen();
    }    
    else if (argc == 5) {
        int x, y, w, h;
        if (JS_ToInt32(ctx, &x, argv[1])) {
            return JS_ThrowTypeError(ctx, "arg1 error");
        }
        else {
            win->x = x;
        }
        if (JS_ToInt32(ctx, &y, argv[2])) {
            return JS_ThrowTypeError(ctx, "arg2 error");
        }
        else {
            win->y = y;
        }
        if (JS_ToInt32(ctx, &w, argv[3])) {
            return JS_ThrowTypeError(ctx, "arg3 error");
        }
        else {
            win->w = w;
        }
        if (JS_ToInt32(ctx, &h, argv[4])) {
            return JS_ThrowTypeError(ctx, "arg4 error");
        }
        else {
            win->h = h;
        }
        win->initWindow(title);
    }
    else {
        win->initWindow(title);
        win->centerScreen();
    }
    win->initCanvas();
    win->show();
	JS_SetOpaque(obj, win);
	return obj;
}

void Win::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &jsWinClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "setPos", JS_NewCFunction(ctx, &Win::setPos, "setPos", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setSize", JS_NewCFunction(ctx, &Win::setSize, "setSize", 2));
	JS_SetPropertyStr(ctx, protoInstance, "setPosCenterScreen", JS_NewCFunction(ctx, &Win::setPosCenterScreen, "setPosCenterScreen", 0));
	JS_SetPropertyStr(ctx, protoInstance, "fillColor", JS_NewCFunction(ctx, &Win::fillColor, "fillColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "refresh", JS_NewCFunction(ctx, &Win::refresh, "refresh", 0));
	JS_SetPropertyStr(ctx, protoInstance, "drawRect", JS_NewCFunction(ctx, &Win::drawRect, "drawRect", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "setSize", JS_NewCFunction(ctx, setSize, "setSize", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setIcon", JS_NewCFunction(ctx, setIcon, "setIcon", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setProfile", JS_NewCFunction(ctx, setProfile, "setProfile", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setProxy", JS_NewCFunction(ctx, setProxy, "setProxy", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setPosition", JS_NewCFunction(ctx, setPosition, "setPosition", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setRootFolder", JS_NewCFunction(ctx, setRootFolder, "setRootFolder", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "navigate", JS_NewCFunction(ctx, navigate, "navigate", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "close", JS_NewCFunction(ctx, close, "close", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "destroy", JS_NewCFunction(ctx, destroy, "destroy", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "run", JS_NewCFunction(ctx, run, "run", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "bind", JS_NewCFunction(ctx, bind, "bind", 2));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Win::constructor, jsWinClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, jsWinClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

Win::Win()
{

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
            //obj->mouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONUP:
        {
            //obj->mouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_MOUSELEAVE: {
            //obj->mouseLeave();
            return true;
        }
        case WM_MOUSEMOVE:
        {
            //obj->mouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
        default:
        {
            break;
        }
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Win::initSizePos()
{
    RECT screenRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    x = (screenRect.right - w) / 2;
    y = (screenRect.bottom - h) / 2;
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

Win::~Win()
{
    DeleteDC(hCompatibleDC);
    DeleteObject(bottomHbitmap);
}

void Win::show()
{
    paintWindow();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
}

void Win::centerScreen()
{
    RECT screenRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    x = (screenRect.right - w) / 2;
    y = (screenRect.bottom - h) / 2;
    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOSIZE);
}

void Win::paintWindow()
{
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
    SetWindowPos(win->hwnd, NULL, win->x, win->y, win->w, win->h, SWP_NOZORDER | SWP_NOSIZE);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::setPosCenterScreen(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->centerScreen();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::fillColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    unsigned int color;
    if (JS_ToUint32(ctx, &color, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    win->canvas->clear(color);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::refresh(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    win->paintWindow();
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
