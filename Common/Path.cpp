#include "Path.h"
#include "Rect.h"
#include "include/core/SkRect.h"
#include "Win.h"
#include "JS.h"
#include "Shadow.h"
#include "include/core/SkPoint3.h"
#include "include/utils/SkShadowUtils.h"

namespace {
	static JSClassID id;
	static JSClassDef pathClass = {
		.class_name{"Path"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto path = (Path*)JS_GetOpaque(val, id);
				delete path;
			}
		}
	};
}
Path::Path()
{
	paint.setAntiAlias(true);
}
Path::~Path()
{
}
void Path::RegPathBase(JSContext* ctx, JSValue& protoInstance)
{
	JS_SetPropertyStr(ctx, protoInstance, "lineTo", JS_NewCFunction(ctx, &Path::lineTo, "lineTo", 2));
	JS_SetPropertyStr(ctx, protoInstance, "moveTo", JS_NewCFunction(ctx, &Path::moveTo, "moveTo", 1));
	JS_SetPropertyStr(ctx, protoInstance, "arcTo", JS_NewCFunction(ctx, &Path::arcTo, "moveTo", 1));
	JS_SetPropertyStr(ctx, protoInstance, "reset", JS_NewCFunction(ctx, &Path::reset, "reset", 1));
	JS_SetPropertyStr(ctx, protoInstance, "addRect", JS_NewCFunction(ctx, &Path::addRect, "addRect", 1));
	JS_SetPropertyStr(ctx, protoInstance, "addEllipse", JS_NewCFunction(ctx, &Path::addEllipse, "addEllipse", 1));
}
JSValue Path::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Path();
	JS_SetOpaque(obj, self);
	return obj;
}

void Path::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &pathClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegPathBase(ctx, protoInstance);
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Path::constructor, pathClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, pathClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}


void Path::Paint(Win* win)
{
	if (shadowSize > 0) {
		SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, shadowSize);// 定义阴影与 z 平面的关系    
		SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
		SkShadowUtils::DrawShadow(win->canvas.get(), path, zPlaneParams, lightPos, 2 * shadowSize,
			shadowAmbientColor, shadowSpotColor, 0);
	}
	win->canvas->drawPath(path, paint);
}

JSValue Path::lineTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	path->path.lineTo(x, y);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::arcTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	auto rect = (Rect*)Element::GetPtr(argv[0]);
	double startAngle;
	if (JS_ToFloat64(ctx, &startAngle, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double sweepAngle;
	if (JS_ToFloat64(ctx, &sweepAngle, argv[2])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	bool flag = JS_ToBool(ctx, argv[3]);
	path->path.arcTo(rect->rect,startAngle,sweepAngle,flag);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::moveTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	path->path.moveTo(x, y);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::reset(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	path->path.reset();
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::addRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	auto rect = (Rect*)Element::GetPtr(argv[0]);
	path->path.addRect(rect->rect);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Path::addEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto path = (Path*)Element::GetPtr(thisVal);
	auto rect = (Rect*)Element::GetPtr(argv[0]);
	path->path.addOval(rect->rect);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
