#include "Win.h"
#include "Paint.h"
#include "Path.h"
#include "JS.h"
#include "include/utils/SkShadowUtils.h"
#include "include/core/SkPoint3.h"

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

JSValue Win::drawShadow(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto path = Path::getPtr(argv[0]);
    unsigned int ambientColor;
    if (JS_ToUint32(ctx, &ambientColor, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    unsigned int spotColor;
    if (JS_ToUint32(ctx, &spotColor, argv[2])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, 20);// 定义阴影与 z 平面的关系    
    SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
    SkShadowUtils::DrawShadow(win->canvas.get(), *path, zPlaneParams, lightPos, 20.f, ambientColor, spotColor, 0);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}