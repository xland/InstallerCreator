#include "Win.h"
#include "Paint.h"
#include "JS.h"

JSValue Win::fillColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int color;
    if (JS_ToUint32(ctx, &color, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    win->canvas->clear(color);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    int x, y, w, h;
    if (JS_ToInt32(ctx, &x, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    if (JS_ToInt32(ctx, &y, argv[2])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    if (JS_ToInt32(ctx, &w, argv[3])) {
        return JS_ThrowTypeError(ctx, "arg3 error");
    }
    if (JS_ToInt32(ctx, &h, argv[4])) {
        return JS_ThrowTypeError(ctx, "arg4 error");
    }
    auto paint = Paint::getPtr(argv[0]);
    SkRect rect;
    rect.setXYWH(x, y, w, h);
    win->canvas->drawRect(rect, *paint);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    int x, y, w, h;
    if (JS_ToInt32(ctx, &x, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    if (JS_ToInt32(ctx, &y, argv[2])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    if (JS_ToInt32(ctx, &w, argv[3])) {
        return JS_ThrowTypeError(ctx, "arg3 error");
    }
    if (JS_ToInt32(ctx, &h, argv[4])) {
        return JS_ThrowTypeError(ctx, "arg4 error");
    }
    auto paint = Paint::getPtr(argv[0]);
    SkRect rect;
    rect.setXYWH(x, y, w, h);
    win->canvas->drawOval(rect, *paint);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}