#include "Rect.h"
#include "JS.h"
#include "Win.h"
#include "include/core/SkRRect.h"
#include "include/core/SkPoint3.h"
#include "include/utils/SkShadowUtils.h"

namespace {
	static JSClassID id;
	static JSClassDef paintClass = {
		.class_name{"Rect"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto rect = (Rect*)JS_GetOpaque(val, id);
				delete rect;
			}
		}
	};
}

Rect::Rect()
{
	paint.setAntiAlias(true);
}
void Rect::RegRectBase(JSContext* ctx, JSValue& protoInstance)
{
	JS_SetPropertyStr(ctx, protoInstance, "setLTRB", JS_NewCFunction(ctx, &Rect::setLTRB, "setLTRB", 4));
	JS_SetPropertyStr(ctx, protoInstance, "setXYWH", JS_NewCFunction(ctx, &Rect::setXYWH, "setXYWH", 4));
	JS_SetPropertyStr(ctx, protoInstance, "contains", JS_NewCFunction(ctx, &Rect::contains, "contains", 2));
	JS_SetPropertyStr(ctx, protoInstance, "setBorderRadius", JS_NewCFunction(ctx, &Rect::setBorderRadius, "setBorderRadius", 4));
}
Rect::~Rect()
{
}

void Rect::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &paintClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegRectBase(ctx, protoInstance);
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Rect::constructor, paintClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetPropertyStr(ctx, ctroInstance, "newLTRB", JS_NewCFunction(ctx, &Rect::newLTRB, "newLTRB", 4));
	JS_SetPropertyStr(ctx, ctroInstance, "newXYWH", JS_NewCFunction(ctx, &Rect::newXYWH, "newXYWH", 4));
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, paintClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

void Rect::Paint(Win* win)
{
	if (isRRect) {
        SkVector radii[4]{
            {topLeft, topLeft},  // 左上角
            {topRight, topRight},  // 右上角
            {bottomRight, bottomRight},  // 右下角
            {bottomLeft, bottomLeft}   // 左下角
        };
        SkRRect rr;
        rr.setRectRadii(rect, radii);
        if (shadowSize > 0) {
            SkPath path;
            path.addRRect(rr);
            paintShadow(win, path);

        }
		win->canvas->drawRRect(rr, paint);
	}
	else {
        if (shadowSize > 0) {
            SkPath path;
            path.addRect(rect);
            paintShadow(win, path);
        }
		win->canvas->drawRect(rect, paint);
	}
}

JSValue Rect::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Rect();
	JS_SetOpaque(obj, self);
	return obj;
}

JSValue Rect::newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Rect();
	self->rect.setLTRB(l, t, r, b);
	JS_SetOpaque(obj, self);
	return obj;
}
JSValue Rect::newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h,err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Rect();
	self->rect.setXYWH(x, y, w, h);
	JS_SetOpaque(obj, self);
	return obj;
}
JSValue Rect::setLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (Rect*)Element::GetPtr(thisVal);
    rect->rect.setLTRB(l, t, r, b);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Rect::setXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (Rect*)Element::GetPtr(thisVal);
    rect->rect.setXYWH(x, y, w, h);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Rect::setBorderRadius(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto [r1, r2, r3, r4, err] = get4Arg(ctx, argv);
    if (JS_IsException(err)) {
        return err;
    }
    auto rect = (Rect*)Element::GetPtr(thisVal);
    if (r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0) {
        rect->isRRect = false;
    }
    else {
        rect->isRRect = true;
    }
    rect->topLeft = r1;
    rect->topRight = r2;
    rect->bottomRight = r3;
    rect->bottomLeft = r4;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

void Rect::paintShadow(Win* win, const SkPath& path)
{
    SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, shadowSize);// 定义阴影与 z 平面的关系    
    SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
    SkShadowUtils::DrawShadow(win->canvas.get(), path, zPlaneParams, lightPos, 2 * shadowSize,
        shadowAmbientColor, shadowSpotColor, 0);
}

JSValue Rect::contains(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	double arg1;
	if (JS_ToFloat64(ctx, &arg1, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double arg2;
	if (JS_ToFloat64(ctx, &arg2, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
    //todo 圆角区域没有验证
	auto rect = (Rect*)Element::GetPtr(thisVal);
	auto flag = rect->rect.contains(arg1, arg2);
	return JS_NewBool(ctx, flag);
}
