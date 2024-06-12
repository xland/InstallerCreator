#include "Win.h"
#include "JS.h"
#include "Path.h"

void Win::regSizePos(JSContext* ctx, JSValue& proto) {
    JS_SetPropertyStr(ctx, proto, "setPos", JS_NewCFunction(ctx, &Win::setPos, "setPos", 2));
    JS_SetPropertyStr(ctx, proto, "getPos", JS_NewCFunction(ctx, &Win::getPos, "getPos", 0));
    JS_SetPropertyStr(ctx, proto, "setSize", JS_NewCFunction(ctx, &Win::setSize, "setSize", 2));
    JS_SetPropertyStr(ctx, proto, "getSize", JS_NewCFunction(ctx, &Win::getSize, "getSize", 0));
    //JS_SetPropertyStr(ctx, proto, "setCaptionPath", JS_NewCFunction(ctx, &Win::setCaptionPath, "setCaptionPath", 0));
    JS_SetPropertyStr(ctx, proto, "setPosCenterScreen", JS_NewCFunction(ctx, &Win::setPosCenterScreen, "setPosCenterScreen", 0));
}

JSValue Win::setPos(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
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
    auto win = getPtr(thisVal);
    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ret, "x", JS_NewInt32(ctx, win->x));
    JS_SetPropertyStr(ctx, ret, "y", JS_NewInt32(ctx, win->y));
    return ret;
}

JSValue Win::setSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
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
    auto win = getPtr(thisVal);
    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ret, "w", JS_NewInt32(ctx, win->w));
    JS_SetPropertyStr(ctx, ret, "h", JS_NewInt32(ctx, win->h));
    return ret;
}

//JSValue Win::setCaptionPath(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
//{
//    auto win = getPtr(thisVal);
//    auto path = Path::getPtr(argv[0]);
//    SkMatrix matrix;
//    matrix.setScale(win->scaleFactor, win->scaleFactor);
//    path->transform(matrix, &win->captionPath);
//    return JS::MakeVal(0, JS_TAG_UNDEFINED);
//}

JSValue Win::setPosCenterScreen(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    RECT screenRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    win->x = (screenRect.right - win->w) / 2;
    win->y = (screenRect.bottom - win->h) / 2;
    SetWindowPos(win->hwnd, NULL, win->x, win->y, win->w, win->h, SWP_NOZORDER | SWP_NOSIZE);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}