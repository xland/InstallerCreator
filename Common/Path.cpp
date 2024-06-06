#include "Path.h"
#include "include/core/SkRect.h"
#include "JS.h"


namespace {
	static JSClassID id;
	static JSClassDef paintClass = {
		.class_name{"Path"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto paint = (Path*)JS_GetOpaque(val, id);
				delete paint;
			}
		}
	};
}

JSValue Path::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	SkPath* path = new SkPath();
	JS_SetOpaque(obj, path);
	return obj;
}

void Path::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &paintClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "lineTo", JS_NewCFunction(ctx, &Path::lineTo, "lineTo", 2));
	JS_SetPropertyStr(ctx, protoInstance, "moveTo", JS_NewCFunction(ctx, &Path::moveTo, "moveTo", 1));
	JS_SetPropertyStr(ctx, protoInstance, "reset", JS_NewCFunction(ctx, &Path::reset, "reset", 1));
	JS_SetPropertyStr(ctx, protoInstance, "addRect", JS_NewCFunction(ctx, &Path::addRect, "addRect", 1));
	JS_SetPropertyStr(ctx, protoInstance, "addEllipse", JS_NewCFunction(ctx, &Path::addEllipse, "addEllipse", 1));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Path::constructor, paintClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, paintClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

Path::~Path()
{
}

SkPath* Path::getPtr(JSValue& val)
{
	auto path = (SkPath*)JS_GetOpaque(val, id);
	return path;
}

JSValue Path::lineTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (SkPath*)JS_GetOpaque(thisVal, id);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	path->lineTo(x, y);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::moveTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (SkPath*)JS_GetOpaque(thisVal, id);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	path->moveTo(x, y);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::reset(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (SkPath*)JS_GetOpaque(thisVal, id);
	path->reset();
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::addRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (SkPath*)JS_GetOpaque(thisVal, id);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	double r;
	if (JS_ToFloat64(ctx, &r, argv[2])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double b;
	if (JS_ToFloat64(ctx, &b, argv[3])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	path->addRect(x, y, r, b);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::addEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (SkPath*)JS_GetOpaque(thisVal, id);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	double r;
	if (JS_ToFloat64(ctx, &r, argv[2])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double b;
	if (JS_ToFloat64(ctx, &b, argv[3])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	SkRect rect;
	rect.setLTRB(x, y, r,b);
	path->addOval(rect);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
