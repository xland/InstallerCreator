#include "Win.h"
#include "Paint.h"
#include "Rect.h"
#include "Path.h"
#include "JS.h"
#include "include/utils/SkShadowUtils.h"
#include "include/core/SkPoint3.h"

void Win::regDraw(JSContext* ctx, JSValue& proto) {
    JS_SetPropertyStr(ctx, proto, "fillColor", JS_NewCFunction(ctx, &Win::fillColor, "fillColor", 1));
    JS_SetPropertyStr(ctx, proto, "drawRect", JS_NewCFunction(ctx, &Win::drawRect, "drawRect", 5));
    JS_SetPropertyStr(ctx, proto, "drawEllipse", JS_NewCFunction(ctx, &Win::drawEllipse, "drawEllipse", 5));
    JS_SetPropertyStr(ctx, proto, "drawShadow", JS_NewCFunction(ctx, &Win::drawShadow, "drawShadow", 3));
}

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
    auto paint = Paint::getPtr(argv[0]);
    auto rect = Rect::getPtr(argv[1]);
    win->canvas->drawRect(*rect, *paint);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto paint = Paint::getPtr(argv[0]);
    auto rect = Rect::getPtr(argv[1]);
    win->canvas->drawOval(*rect, *paint);
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
    SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, 30);// 定义阴影与 z 平面的关系    
    SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
    SkShadowUtils::DrawShadow(win->canvas.get(), *path, zPlaneParams, lightPos, 60.f, ambientColor, spotColor, 0);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}