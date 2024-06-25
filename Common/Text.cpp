#include "Text.h"
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/core/SkFontTypes.h>
#include <include/ports/SkTypeface_win.h>

#include "Util.h"
#include "App.h"
#include "Win.h"

namespace {
	static JSClassID id;
	static JSClassDef textClass = {
		.class_name{"Text"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto text = (Text*)JS_GetOpaque(val, id);
				delete text;
			}
		}
	};
}
Text::Text()
{
}
Text::~Text()
{
}

void Text::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &textClass);
	JSValue protoInstance = JS_NewObject(ctx);
	Element::RegBase(ctx, protoInstance);
    TextBase::RegTextBase(ctx, protoInstance);
	JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Text::setText, "setText", 1));
    JS_SetPropertyStr(ctx, protoInstance, "setDecoration", JS_NewCFunction(ctx, &Text::setDecoration, "setDecoration", 2));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Text::constructor, textClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, textClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

JSValue Text::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Text();
	JS_SetOpaque(obj, self);
	return obj;
}

std::tuple<int, float> Text::getTextCursorPos(const int& x1)
{
    auto width = x1 - x;
    if (width < 0) width = 0;
    SkRect subRect;
    float tempWidth{0};
    for (size_t i = 1; i < text.length(); i++)
    {
        auto str = text.substr(0, i);
        auto data = str.data();
        auto length = wcslen(data) * 2;
        font->measureText(data, length, SkTextEncoding::kUTF16, &subRect);
        if (i==1 && width < subRect.width()/2) {
            return { 0,x };
        } else if (subRect.width() > width) {
            return { i,x + subRect.width() + subRect.fLeft * 2 };
        }
        tempWidth = subRect.width();
    }
    return { text.length(),x + subRect.width() + subRect.fLeft * 2};
}

float Text::getTextCursorPosByWordIndex(const int& index)
{
    SkRect subRect;
    auto str = text.substr(0, index);
    auto data = str.data();
    auto length = wcslen(data) * 2;
    font->measureText(data, length, SkTextEncoding::kUTF16, &subRect);
    return x + subRect.width() + subRect.fLeft * 2;
}

void Text::resetLineRect()
{
	if (!font.get()) {
		font = App::GetDefaultTextFont();
    }
    font->setSize(fontSize);
	auto length = wcslen(text.data()) * 2;
    if (length == 0) {
        std::wstring temp{ L"中" };
        font->measureText(temp.data(), 2, SkTextEncoding::kUTF16, &lineRect);
    }
    else {
        font->measureText(text.data(), length, SkTextEncoding::kUTF16, &lineRect);
    }
}

JSValue Text::setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	const char* strData = JS_ToCString(ctx, argv[0]);
	if (!strData) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	auto str = Util::ConvertToWStr(strData);
	JS_FreeCString(ctx, strData);
	auto obj = (Text*)Element::GetPtr(thisVal);
	if (obj->text != str) {
		obj->text = str;
		obj->resetLineRect();
	}
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Text::setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    double decorationSize;
    if (JS_ToFloat64(ctx, &decorationSize, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    unsigned int decorationColor;
    if (JS_ToUint32(ctx, &decorationColor, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    auto text = (Text*)GetPtr(thisVal);
    text->decorationSize = decorationSize;
    text->decorationColor = decorationColor;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
void Text::Paint(Win* win)
{
    if (decorationSize > 0) {
        SkPaint paint;
        paint.setColor(decorationColor);
        paint.setStroke(true);
        paint.setStrokeWidth(decorationSize);
        auto bottom = lineRect.fBottom+y+1;
        auto right = lineRect.fRight+x-1;
        win->canvas->drawLine(x, bottom, right, bottom, paint);
    }    
	auto length = wcslen(text.data()) * 2;
	win->canvas->drawSimpleText(text.c_str(), length, SkTextEncoding::kUTF16, x, y, *font.get(), paint);
}

