#include "Rect.h"
#include "JS.h"

static JSClassID id;
static JSClassDef paintClass = {
	.class_name{"Rect"},
	.finalizer{[](JSRuntime* rt, JSValue val) {
			auto rect = (SkRect*)JS_GetOpaque(val, id);
			delete rect;
		}
	}
};

void Rect::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &paintClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "setLTRB", JS_NewCFunction(ctx, &Rect::setLTRB, "setLTRB", 4));
	JS_SetPropertyStr(ctx, protoInstance, "setXYWH", JS_NewCFunction(ctx, &Rect::setXYWH, "setXYWH", 4));
	JS_SetPropertyStr(ctx, protoInstance, "contains", JS_NewCFunction(ctx, &Rect::contains, "contains", 2));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Rect::constructor, paintClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, "newRectLTRB", JS_NewCFunction(ctx, &Rect::newRectLTRB, "newRectLTRB", 4));
	JS_SetPropertyStr(ctx, global, "newRectXYWH", JS_NewCFunction(ctx, &Rect::newRectXYWH, "newRectXYWH", 4));
	JS_SetPropertyStr(ctx, global, paintClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

Rect::~Rect()
{
}

SkRect* Rect::getPtr(JSValue& val)
{
	auto rect = (SkRect*)JS_GetOpaque(val, id);
	return rect;
}

JSValue Rect::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	SkRect* rect = new SkRect();
	JS_SetOpaque(obj, rect);
	return obj;
}

JSValue Rect::newRectLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	SkRect* rect = new SkRect();
	rect->setLTRB(l, t, r, b);
	JS_SetOpaque(obj, rect);
	return obj;
}

JSValue Rect::newRectXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h,err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	SkRect* rect = new SkRect();
	rect->setXYWH(x, y, w, h);
	JS_SetOpaque(obj, rect);
	return obj;
}

JSValue Rect::setLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (SkRect*)JS_GetOpaque(thisVal, id);
	rect->setLTRB(l, t, r, b);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Rect::setXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	auto rect = (SkRect*)JS_GetOpaque(thisVal, id);
	rect->setXYWH(x, y, w, h);
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
	auto rect = (SkRect*)JS_GetOpaque(thisVal, id);
	auto flag = rect->contains(arg1, arg2);
	return JS_NewBool(ctx, flag);
}

std::tuple<double, double, double, double, JSValue> Rect::get4Arg(JSContext* ctx, JSValueConst* argv)
{
	JSValue err = JS::MakeVal(0, JS_TAG_UNDEFINED);
	double arg1;
	if (JS_ToFloat64(ctx, &arg1, argv[0])) {
		err = JS_ThrowTypeError(ctx, "arg0 error");
	}
	double arg2;
	if (JS_ToFloat64(ctx, &arg2, argv[1])) {
		err = JS_ThrowTypeError(ctx, "arg1 error");
	}
	double arg3;
	if (JS_ToFloat64(ctx, &arg3, argv[2])) {
		err = JS_ThrowTypeError(ctx, "arg2 error");
	}
	double arg4;
	if (JS_ToFloat64(ctx, &arg4, argv[3])) {
		err = JS_ThrowTypeError(ctx, "arg3 error");
	}
	return {arg1,arg2,arg3,arg4,err};
}
