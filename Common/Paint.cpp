#include "Paint.h"
#include "JS.h"
#include "include/effects/SkGradientShader.h"
#include <vector>

namespace {	
	static JSClassID id;
	static JSClassDef paintClass = {
		.class_name{"Paint"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto paint = (SkPaint*)JS_GetOpaque(val, id);
				delete paint;
			}
		}
	};
}

void Paint::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &paintClass);
	JSValue protoInstance = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, protoInstance, "setStroke", JS_NewCFunction(ctx, &Paint::setStroke, "setStroke", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeWidth", JS_NewCFunction(ctx, &Paint::setStrokeWidth, "setStrokeWidth", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setColor", JS_NewCFunction(ctx, &Paint::setColor, "setColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setAntiAlias", JS_NewCFunction(ctx, &Paint::setAntiAlias, "setAntiAlias", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeCap", JS_NewCFunction(ctx, &Paint::setStrokeCap, "setStrokeCap", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeJoin", JS_NewCFunction(ctx, &Paint::setStrokeJoin, "setStrokeJoin", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setBlendMode", JS_NewCFunction(ctx, &Paint::setBlendMode, "setBlendMode", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setLinearShader", JS_NewCFunction(ctx, &Paint::setLinearShader, "setLinearShader", 0));
	JS_SetPropertyStr(ctx, protoInstance, "setRadialShader", JS_NewCFunction(ctx, &Paint::setRadialShader, "setRadialShader", 0));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Paint::constructor, paintClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, paintClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

JSValue Paint::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	SkPaint* paint = new SkPaint();
	JS_SetOpaque(obj, paint);
	return obj;
}

JSValue Paint::setStroke(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	bool flag = JS_ToBool(ctx, argv[0]);
	paint->setStroke(flag);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setStrokeWidth(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	double strokeWidth;
	if (JS_ToFloat64(ctx, &strokeWidth, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	paint->setStrokeWidth(strokeWidth);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	unsigned int color;
	if (JS_ToUint32(ctx, &color, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	paint->setColor(color);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setLinearShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	uint32_t length = 0;
	JS_ToUint32(ctx, &length, JS_GetPropertyStr(ctx, argv[3], "length"));
	std::vector<SkColor> colors;
	for (uint32_t i = 0; i < length; ++i) {
		JSValue element = JS_GetPropertyUint32(ctx, argv[3], i);
		unsigned int value = 0;
		JS_ToUint32(ctx, &value, element);
		colors.push_back(value);
		JS_FreeValue(ctx, element);
	}
	SkPoint pos[2]{ SkPoint::Make(100, 100) ,SkPoint::Make(600, 600) };
	auto shader = SkGradientShader::MakeLinear(pos, colors.data(), nullptr, length, SkTileMode::kClamp);
	paint->setShader(shader);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setRadialShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	uint32_t length = 0;
	JS_ToUint32(ctx, &length, JS_GetPropertyStr(ctx, argv[3], "length"));
	std::vector<SkColor> colors;
	for (uint32_t i = 0; i < length; ++i) {
		JSValue element = JS_GetPropertyUint32(ctx, argv[3], i);
		unsigned int value = 0;
		JS_ToUint32(ctx, &value, element);
		colors.push_back(value);
		JS_FreeValue(ctx, element);
	}

	auto shader = SkGradientShader::MakeRadial(SkPoint::Make(400, 400), 300, colors.data(), nullptr, length, SkTileMode::kClamp);
	paint->setShader(shader);

	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setAntiAlias(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	bool flag = JS_ToBool(ctx, argv[0]);
	paint->setAntiAlias(flag);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setStrokeCap(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	unsigned int cap;
	if (JS_ToUint32(ctx, &cap, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	paint->setStrokeCap((SkPaint::Cap)cap);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setStrokeJoin(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	unsigned int join;
	if (JS_ToUint32(ctx, &join, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	paint->setStrokeJoin((SkPaint::Join)join);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Paint::setBlendMode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto paint = (SkPaint*)JS_GetOpaque(thisVal, id);
	unsigned int blendMode;
	if (JS_ToUint32(ctx, &blendMode, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	paint->setBlendMode((SkBlendMode)blendMode);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

SkPaint* Paint::getPtr(JSValue& val)
{
	auto paint = (SkPaint*)JS_GetOpaque(val, id);
	return paint;
}
