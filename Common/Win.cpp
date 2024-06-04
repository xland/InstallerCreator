#include "Win.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <map>
#include <string>
#include <format>

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
	auto win = new Win();
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
	JS_SetPropertyStr(ctx, protoInstance, "setPosCenterScreen", JS_NewCFunction(ctx, &Win::setPosCenterScreen, "setPosCenterScreen", 0));
	JS_SetPropertyStr(ctx, protoInstance, "fillColor", JS_NewCFunction(ctx, &Win::fillColor, "fillColor", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setPublic", JS_NewCFunction(ctx, setPublic, "setPublic", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "getUrl", JS_NewCFunction(ctx, getUrl, "getUrl", 0));
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
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Win::constructor, jsWinClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, jsWinClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

Win::Win()
{
    initWindow();
    initCanvas();
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
        case WM_ERASEBKGND: {
            return false;
        }
        case WM_PAINT: {
            //obj->paintWindow();
            return true;
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


void Win::initWindow()
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
	this->hwnd = CreateWindowEx(WS_EX_LAYERED, className.c_str(), className.c_str(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
}

Win::~Win()
{
}

void Win::paintWindow()
{
    PAINTSTRUCT ps;
    auto dc = BeginPaint(hwnd, &ps);
    auto c = canvas.get();
    BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(surfaceMemory.get());
    StretchDIBits(dc, 0, 0, w, h, 0, 0, w, h, bmpInfo->bmiColors, bmpInfo, DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(hwnd, dc);
    EndPaint(hwnd, &ps);
    //surfaceMemory.reset(0); //实践证明这样即节省内存，速度也不会慢
}

void Win::initCanvas()
{
    size_t bmpSize = sizeof(BITMAPINFOHEADER) + w * h * sizeof(uint32_t);
    surfaceMemory.reset(bmpSize);
    BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(surfaceMemory.get());
    ZeroMemory(bmpInfo, sizeof(BITMAPINFO));
    bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo->bmiHeader.biWidth = w;
    bmpInfo->bmiHeader.biHeight = -h;
    bmpInfo->bmiHeader.biPlanes = 1;
    bmpInfo->bmiHeader.biBitCount = 32;
    bmpInfo->bmiHeader.biCompression = BI_RGB;
    void* pixels = bmpInfo->bmiColors;
    SkImageInfo info = SkImageInfo::MakeN32Premul(w, h);
    auto temp = SkCanvas::MakeRasterDirect(info, pixels, 4 * w);
    canvas = std::move(temp);
}

JSValue Win::setPos(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    return JSValue();
}

JSValue Win::setPosCenterScreen(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    return JSValue();
}

JSValue Win::fillColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = (Win*)JS_GetOpaque(thisVal, id);
    return JSValue();
}
