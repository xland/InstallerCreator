#include "Rect.h"
#include "JS.h"
#include "Win.h"
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
	if (shadowSize > 0) {
		SkPath path;
		if (rrect.isEmpty()) {
			path.addRect(rect);
		}
		else {
			path.addRRect(rrect);
		}
		SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, shadowSize);// 定义阴影与 z 平面的关系    
		SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
		SkShadowUtils::DrawShadow(win->canvas.get(), path, zPlaneParams, lightPos, 2 * shadowSize,
			shadowAmbientColor, shadowSpotColor, 0);
	}
	if (rrect.isEmpty()) {
		win->canvas->drawRect(rect, paint);
	}
	else {
		win->canvas->drawRRect(rrect, paint);
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

JSValue Rect::setBorderRadius(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [r1, r2, r3, r4, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (Rect*)Element::GetPtr(thisVal);
	SkVector radii[4] {
		{r1, r1},  // 左上角
		{r2, r2},  // 右上角
		{r3, r3},  // 右下角
		{r4, r4}   // 左下角
	};
	rect->rrect.setRectRadii(rect->rect, radii);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Rect::setLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (Rect*)Element::GetPtr(thisVal);
	if (rect->rrect.isEmpty()) {
		rect->rect.setLTRB(l, t, r, b);
	}
	else {
		auto radii1 = rect->rrect.radii(SkRRect::Corner::kUpperLeft_Corner);
		auto radii2 = rect->rrect.radii(SkRRect::Corner::kUpperRight_Corner);
		auto radii3 = rect->rrect.radii(SkRRect::Corner::kLowerRight_Corner);
		auto radii4 = rect->rrect.radii(SkRRect::Corner::kLowerLeft_Corner);
		SkVector radii[4]{ radii1 ,radii2,radii3,radii4 };
		SkRect tempR;
		tempR.setLTRB(l, t, r, b);
		rect->rrect.setRectRadii(tempR,radii);
	}
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Rect::setXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (Rect*)Element::GetPtr(thisVal);
	auto radii = rect->rrect.radii(SkRRect::Corner::kUpperLeft_Corner);
	if (rect->rrect.isEmpty()) {
		rect->rect.setXYWH(x, y, w, h);
	}
	else {
		auto radii1 = rect->rrect.radii(SkRRect::Corner::kUpperLeft_Corner);
		auto radii2 = rect->rrect.radii(SkRRect::Corner::kUpperRight_Corner);
		auto radii3 = rect->rrect.radii(SkRRect::Corner::kLowerRight_Corner);
		auto radii4 = rect->rrect.radii(SkRRect::Corner::kLowerLeft_Corner);
		SkVector radii[4]{ radii1 ,radii2,radii3,radii4 };
		SkRect tempR;
		tempR.setXYWH(x, y, w, h);
		rect->rrect.setRectRadii(tempR, radii);
	}
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
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
	auto rect = (Rect*)Element::GetPtr(thisVal);
	auto flag = rect->rect.contains(arg1, arg2);
	return JS_NewBool(ctx, flag);
}
