#include "Element.h"
#include "include/effects/SkGradientShader.h"
#include "Console.h"

Element::Element()
{
}
Element::~Element()
{
}
Element* Element::GetPtr(const JSValue& val) {
	JSClassID classId{ 0 };
	auto element = (Element*)JS_GetAnyOpaque(val, &classId);
	if (!element) {
		Console::Log("can not get element ptr");
	}
	return element;
}
void Element::RegBase(JSContext* ctx,JSValue& protoInstance)
{
	JS_SetPropertyStr(ctx, protoInstance, "setId", JS_NewCFunction(ctx, &Element::setId, "setId", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setVisible", JS_NewCFunction(ctx, &Element::setVisible, "setVisible", 1));
	JS_SetPropertyStr(ctx, protoInstance, "getVisible", JS_NewCFunction(ctx, &Element::getVisible, "getVisible", 0));
	JS_SetPropertyStr(ctx, protoInstance, "setStroke", JS_NewCFunction(ctx, &Element::setStroke, "setStroke", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeWidth", JS_NewCFunction(ctx, &Element::setStrokeWidth, "setStrokeWidth", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setColor", JS_NewCFunction(ctx, &Element::setColor, "setColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeCap", JS_NewCFunction(ctx, &Element::setStrokeCap, "setStrokeCap", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setStrokeJoin", JS_NewCFunction(ctx, &Element::setStrokeJoin, "setStrokeJoin", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setBlendMode", JS_NewCFunction(ctx, &Element::setBlendMode, "setBlendMode", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setLinearShader", JS_NewCFunction(ctx, &Element::setLinearShader, "setLinearShader", 0));
	JS_SetPropertyStr(ctx, protoInstance, "setRadialShader", JS_NewCFunction(ctx, &Element::setRadialShader, "setRadialShader", 0));
	JS_SetPropertyStr(ctx, protoInstance, "setShadowSize", JS_NewCFunction(ctx, &Element::setShadowSize, "setShadowSize", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setShadowAmbientColor", JS_NewCFunction(ctx, &Element::setShadowAmbientColor, "setShadowAmbientColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setShadowSpotColor", JS_NewCFunction(ctx, &Element::setShadowSpotColor, "setShadowSpotColor", 1));
}
JSValue Element::setId(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	const char* strData = JS_ToCString(ctx, argv[0]);
	if (!strData) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->idStr = std::string(strData);
	JS_FreeCString(ctx, strData);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Element::setVisible(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	bool flag = JS_ToBool(ctx, argv[0]);
	ele->visible = flag;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Element::getVisible(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	return JS_NewBool(ctx,ele->visible);
}
JSValue Element::setShadowAmbientColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	unsigned int ambientColor;
	if (JS_ToUint32(ctx, &ambientColor, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	auto ele = GetPtr(thisVal);
	ele->shadowAmbientColor = ambientColor;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Element::setShadowSpotColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	unsigned int spotColor;
	if (JS_ToUint32(ctx, &spotColor, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	auto ele = GetPtr(thisVal);
	ele->shadowSpotColor = spotColor;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setStroke(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	bool flag = JS_ToBool(ctx, argv[0]);
	ele->paint.setStroke(flag);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setStrokeWidth(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	double strokeWidth;
	if (JS_ToFloat64(ctx, &strokeWidth, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}

	ele->paint.setStrokeWidth(strokeWidth);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setShadowSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	double shadowSize;
	if (JS_ToFloat64(ctx, &shadowSize, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->shadowSize = shadowSize;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	unsigned int color;
	if (JS_ToUint32(ctx, &color, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->paint.setColor(color);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setLinearShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
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
	SkPoint pos[2]{ SkPoint::Make(300, 300) ,SkPoint::Make(500, 500) };
	auto shader = SkGradientShader::MakeLinear(pos, colors.data(), nullptr, length, SkTileMode::kClamp);
	ele->paint.setShader(shader);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setRadialShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
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
	ele->paint.setShader(shader);

	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setStrokeCap(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	unsigned int cap;
	if (JS_ToUint32(ctx, &cap, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->paint.setStrokeCap((SkPaint::Cap)cap);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setStrokeJoin(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	unsigned int join;
	if (JS_ToUint32(ctx, &join, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->paint.setStrokeJoin((SkPaint::Join)join);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Element::setBlendMode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto ele = GetPtr(thisVal);
	unsigned int blendMode;
	if (JS_ToUint32(ctx, &blendMode, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	ele->paint.setBlendMode((SkBlendMode)blendMode);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

std::tuple<double, double, double, double, JSValue> Element::get4Arg(JSContext* ctx, JSValueConst* argv)
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
	return { arg1,arg2,arg3,arg4,err };
}


