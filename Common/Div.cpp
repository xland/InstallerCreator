#include "Div.h"
#include "include/utils/SkTextUtils.h"
#include "src/base/SkUTF.h"
#include "Util.h"
#include "Win.h"

namespace {
	static JSClassID id;
	static JSClassDef paintClass = {
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
JSValue Div::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Div();
	JS_SetOpaque(obj, self);
	return obj;
}
Div::~Div()
{
}


void Div::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &paintClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegRectBase(ctx, protoInstance);
	JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Div::setText, "setText", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setAlign", JS_NewCFunction(ctx, &Div::setAlign, "setAlign", 2));
	JS_SetPropertyStr(ctx, protoInstance, "setTextColor", JS_NewCFunction(ctx, &Div::setTextColor, "setTextColor", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &Div::setFontSize, "setFontSize", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setFontFamily", JS_NewCFunction(ctx, &Div::setFontFamily, "setFontFamily", 1));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Div::constructor, paintClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetPropertyStr(ctx, ctroInstance, "newLTRB", JS_NewCFunction(ctx, &Div::newLTRB, "newLTRB", 4));
	JS_SetPropertyStr(ctx, ctroInstance, "newXYWH", JS_NewCFunction(ctx, &Div::newXYWH, "newXYWH", 4));
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, paintClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
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


void Div::Paint(Win* win)
{
	Rect::Paint(win);
	font->setSize(fontSize);
	SkPaint textPaint;
	textPaint.setAntiAlias(true);
	textPaint.setColor(color);
	if (isIcon) {
		SkRect lineRect;
		font->measureText(iconCode, strlen(iconCode), SkTextEncoding::kUTF8, &lineRect);
		float left{ rect.fLeft - lineRect.fLeft };
		float top{ rect.fTop - lineRect.fTop };
		if (verticalAlign == 1) {
			top += (rect.height() - lineRect.height()) / 2;
		}
		else if (verticalAlign == 2) {
			top = rect.fBottom - lineRect.fBottom;
		}
		if (horizontalAlign == 1) {
			left += (rect.width() - lineRect.width()) / 2;
		}
		else if (horizontalAlign == 2) {
			left = rect.fRight - lineRect.width();
		}
		win->canvas->drawSimpleText(iconCode, strlen(iconCode), SkTextEncoding::kUTF8, left, top, *font, paint);
	}
	else
	{
		auto length = wcslen(text.data()) * 2;
		SkRect lineRect;
		font->measureText(text.data(), length, SkTextEncoding::kUTF16, &lineRect);
		float left{ rect.fLeft - lineRect.fLeft };
		float top{ rect.fTop - lineRect.fTop };
		if (verticalAlign == 1) {
			top += (rect.height() - lineRect.height()) / 2;
		}
		else if (verticalAlign == 2) {
			top = rect.fBottom - lineRect.fBottom;
		}
		if (horizontalAlign == 1) {
			left += (rect.width() - lineRect.width()) / 2;
		}
		else if (horizontalAlign == 2) {
			left = rect.fRight - lineRect.width();
		}
		SkTextUtils::Draw(win->canvas.get(), text.data(), length,
			SkTextEncoding::kUTF16,
			left, top,
			*font, textPaint,
			SkTextUtils::kLeft_Align);
	}
}

JSValue Div::setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto div = (Div*)JS_GetOpaque(thisVal, id);
	const char* strData = JS_ToCString(ctx, argv[0]);
	if (!strData) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	if (strData[0] == '\\' && strData[1] == 'u') {
		div->isIcon = true;
		div->iconCode = strData;
		//std::string_view cstr_view(strData);
		//auto icon = std::u8string(cstr_view.begin(), cstr_view.end());
		//auto b = std::u8string(u8"\ue6e6");
		//div->iconCode = std::string(icon.begin(), icon.end());
		return JS::MakeVal(0, JS_TAG_UNDEFINED);
	}
	div->text = Util::ConvertToWideChar(strData); 
	JS_FreeCString(ctx, strData);
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
	div->horizontalAlign = vAlign;
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
	div->font = JS::GetFont(name);
	JS_FreeCString(ctx, fontName);
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
