#include "Div.h"
#include "include/utils/SkTextUtils.h"
#include <string>
#include <format>
#include <iostream>
#include "Util.h"
#include "Win.h"
#include "App.h"

namespace {
	static JSClassID id;
	static JSClassDef divClass = {
		.class_name{"Div"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto div = (Div*)JS_GetOpaque(val, id);
				delete div;
			}
		}
	};
}
Div::Div()
{
}
Div::~Div()
{
}
JSValue Div::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Div();
	JS_SetOpaque(obj, self);
	return obj;
}
void Div::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &divClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegRectBase(ctx, protoInstance);
	JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Div::setText, "setText", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setIcon", JS_NewCFunction(ctx, &Div::setIcon, "setIcon", 1));
	JS_SetPropertyStr(ctx, protoInstance, "getText", JS_NewCFunction(ctx, &Div::getText, "getText", 0));
	JS_SetPropertyStr(ctx, protoInstance, "getIcon", JS_NewCFunction(ctx, &Div::getIcon, "getIcon", 0));

	JS_SetPropertyStr(ctx, protoInstance, "setDecoration", JS_NewCFunction(ctx, &Div::setDecoration, "setDecoration", 2));
	JS_SetPropertyStr(ctx, protoInstance, "setAlign", JS_NewCFunction(ctx, &Div::setAlign, "setAlign", 2));
	JS_SetPropertyStr(ctx, protoInstance, "setIndent", JS_NewCFunction(ctx, &Div::setIndent, "setIndent", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setTextColor", JS_NewCFunction(ctx, &Div::setTextColor, "setTextColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &Div::setFontSize, "setFontSize", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setFontFamily", JS_NewCFunction(ctx, &Div::setFontFamily, "setFontFamily", 1));
	JS_SetPropertyStr(ctx, protoInstance, "onMouseEnter", JS_NewCFunction(ctx, &Div::onMouseEnter, "onMouseEnter", 1));
	JS_SetPropertyStr(ctx, protoInstance, "onMouseLeave", JS_NewCFunction(ctx, &Div::onMouseLeave, "onMouseLeave", 1));
	JS_SetPropertyStr(ctx, protoInstance, "onMouseDown", JS_NewCFunction(ctx, &Div::onMouseDown, "onMouseDown", 1));
	JS_SetPropertyStr(ctx, protoInstance, "onMouseUp", JS_NewCFunction(ctx, &Div::onMouseUp, "onMouseUp", 1));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Div::constructor, divClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetPropertyStr(ctx, ctroInstance, "newLTRB", JS_NewCFunction(ctx, &Div::newLTRB, "newLTRB", 4));
	JS_SetPropertyStr(ctx, ctroInstance, "newXYWH", JS_NewCFunction(ctx, &Div::newXYWH, "newXYWH", 4));
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, divClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}
void Div::Paint(Win* win)
{
	Rect::Paint(win);
	if (iconCode > 0) {
		paintIcon(win);
	}
	else if (!text.empty())
	{
		piantText(win);
	}
}
void Div::paintIcon(Win* win)
{
	font->setSize(fontSize);
	SkPaint textPaint;
	textPaint.setAntiAlias(true);
	textPaint.setColor(color);

	std::u8string utf8Str;
	utf8Str += static_cast<char8_t>((iconCode >> 12) | 0xE0);
	utf8Str += static_cast<char8_t>(((iconCode >> 6) & 0x3F) | 0x80);
	utf8Str += static_cast<char8_t>((iconCode & 0x3F) | 0x80);
	std::string iconStr(utf8Str.begin(), utf8Str.end());

	SkRect lineRect;
	font->measureText(iconStr.c_str(), 4, SkTextEncoding::kUTF8, &lineRect);
	auto [l, t, r, b] = getTextPos(lineRect);
	win->canvas->drawSimpleText(iconStr.c_str(), 4, SkTextEncoding::kUTF8, l, t, *font, textPaint);
	if (decorationSize > 0) {
		paintDecoration(win, l, r, b);
	}
}
void Div::piantText(Win* win)
{
	font->setSize(fontSize);
	SkPaint textPaint;
	textPaint.setAntiAlias(true);
	textPaint.setColor(color);
	auto length = wcslen(text.data()) * 2;
	SkRect lineRect;
	font->measureText(text.data(), length, SkTextEncoding::kUTF16, &lineRect);
	auto [l,t,r,b] = getTextPos(lineRect);
	win->canvas->drawSimpleText(text.c_str(), length, SkTextEncoding::kUTF16, l, t, *font, textPaint);
	if (decorationSize > 0) {
		paintDecoration(win, l,r, b);
	}
}
void Div::paintDecoration(Win* win, float l, float r,float b)
{
	SkPaint paint;
	paint.setAntiAlias(false);
	paint.setColor(decorationColor);
	paint.setStroke(true);
	paint.setStrokeWidth(decorationSize);
	win->canvas->drawLine(l, b, r, b, paint);
}
void Div::MouseMove(const float& x, const float& y)
{
	auto flag = rect.contains(x, y);
	if (flag == isMouseEnter) {
		return;
	}
	isMouseEnter = flag;
	auto ctx = JS::GetCtx();
	if (flag) {
		if (!JS_IsFunction(ctx, mouseEnterCB)) {
			return;
		}
		JSValue ret = JS_Call(ctx, mouseEnterCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
		JS_FreeValue(ctx, ret);
	}
	else {
		if (!JS_IsFunction(ctx, mouseLeaveCB)) {
			return;
		}
		JSValue ret = JS_Call(ctx, mouseLeaveCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
		JS_FreeValue(ctx, ret);
	}
}
void Div::MouseDown()
{
	if (!isMouseEnter) {
		return;
	}
	auto ctx = JS::GetCtx();
	if (!JS_IsFunction(ctx, mouseDownCB)) {
		return;
	}
	isMouseDown = true;
	JSValue ret = JS_Call(ctx, mouseDownCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
	JS_FreeValue(ctx, ret);
}
void Div::MouseUp()
{
	if (!isMouseDown) {
		return;
	}
	auto ctx = JS::GetCtx();
	if (!JS_IsFunction(ctx, mouseUpCB)) {
		return;
	}
	isMouseDown = false;
	JSValue ret = JS_Call(ctx, mouseUpCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
	JS_FreeValue(ctx, ret);
}
void Div::Dispose()
{
	auto ctx = JS::GetCtx();
	JS_FreeValue(ctx, mouseEnterCB);
	JS_FreeValue(ctx, mouseLeaveCB);
	JS_FreeValue(ctx, mouseDownCB);
	JS_FreeValue(ctx, mouseUpCB);
	//JS_RunGC(JS::GetRt());
}
std::tuple<float, float, float, float> Div::getTextPos(SkRect& lineRect)
{
	float left{ rect.fLeft - lineRect.fLeft };
	float top{ rect.fTop - lineRect.fTop };
	float w{ lineRect.width() };
	float h{ lineRect.height() };
	if (verticalAlign == 0) {
		top += indentVertical;
	}
	else if (verticalAlign == 1) {
		top += (rect.height() - h) / 2;
	}
	else if (verticalAlign == 2) {
		top = rect.fBottom - lineRect.fBottom - indentVertical;
	}

	if (horizontalAlign == 0) {
		left += indentHorizontal;
	}
	else if (horizontalAlign == 1) {
		left += (rect.width() - w) / 2;
	}
	else if (horizontalAlign == 2) {
		left = rect.fRight - w - indentHorizontal;
	}
	return { left,top,
		left + lineRect.fLeft*2 + lineRect.width(),
		top + lineRect.fTop + lineRect.height()+1};
}
JSValue Div::newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Div();
	self->rect.setLTRB(l, t, r, b);
	JS_SetOpaque(obj, self);
	return obj;
}
JSValue Div::newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Div();
	self->rect.setXYWH(x, y, w, h);
	JS_SetOpaque(obj, self);
	return obj;
}
JSValue Div::setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);	
	const char* strData = JS_ToCString(ctx, argv[0]);
	if (!strData) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	div->text = Util::ConvertToWStr(strData);
	JS_FreeCString(ctx, strData);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::getText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	auto str = Util::ConvertToStr(div->text);
	return JS_NewString(ctx, str.data());
}
JSValue Div::setIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	uint32_t iconCode;
	if (JS_ToUint32(ctx, &iconCode, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	div->iconCode = iconCode;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::getIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	return JS_NewUint32(ctx,div->iconCode);
}
JSValue Div::setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	double indentVertical;
	if (JS_ToFloat64(ctx, &indentVertical, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	double indentHorizontal;
	if (JS_ToFloat64(ctx, &indentHorizontal, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	div->indentVertical = indentVertical;
	div->indentHorizontal = indentHorizontal;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	unsigned int vAlign;
    if (JS_ToUint32(ctx, &vAlign, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    unsigned int hAlign;
    if (JS_ToUint32(ctx, &hAlign, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->verticalAlign = vAlign;
	div->horizontalAlign = hAlign;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setTextColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	unsigned int color;
	if (JS_ToUint32(ctx, &color, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	div->color = color;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	double fontSize;
	if (JS_ToFloat64(ctx, &fontSize, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	div->fontSize = fontSize;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	const char* fontName = JS_ToCString(ctx, argv[0]);
	if (!fontName) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	auto name = std::string{ fontName };
	div->font = App::GetFont(name);
	JS_FreeCString(ctx, fontName);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	double decorationSize;
	if (JS_ToFloat64(ctx, &decorationSize, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	unsigned int decorationColor;
	if (JS_ToUint32(ctx, &decorationColor, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->decorationSize = decorationSize;
	div->decorationColor = decorationColor;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->mouseEnterCB = JS_DupValue(ctx, argv[0]);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->mouseLeaveCB = JS_DupValue(ctx, argv[0]);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->mouseDownCB = JS_DupValue(ctx, argv[0]);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	div->mouseUpCB = JS_DupValue(ctx, argv[0]);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}