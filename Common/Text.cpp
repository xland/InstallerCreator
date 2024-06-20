#include "Text.h"
#include "Util.h"
#include "App.h"
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/core/SkFontTypes.h>
#include <include/ports/SkTypeface_win.h>

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
	RegBase(ctx, protoInstance);
	JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &Text::setFontSize, "setFontSize", 1));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Text::constructor, textClass.class_name, 5, JS_CFUNC_constructor, 0);
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

	const char* strData = JS_ToCString(ctx, argv[0]);
	if (!strData) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	self->text = Util::ConvertToWStr(strData);
	JS_FreeCString(ctx, strData);

	double x;
	if (JS_ToFloat64(ctx, &x, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}	

	double y;
	if (JS_ToFloat64(ctx, &y, argv[1])) {
		return JS_ThrowTypeError(ctx, "arg1 error");
	}

	auto font = App::GetDefaultTextFont();
	auto length = wcslen(self->text.data()) * 2;
	font->measureText(self->text.data(), length, SkTextEncoding::kUTF16, &self->textRect);
	self->textRect.offsetTo(0, 0);

	JS_SetOpaque(obj, self);
	return obj;
}

JSValue Text::setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto text = (Text*)Element::GetPtr(thisVal);
	double fontSize;
	if (JS_ToFloat64(ctx, &fontSize, argv[0])) {
		return JS_ThrowTypeError(ctx, "arg0 error");
	}
	text->fontSize = fontSize;
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

void Text::Paint(Win* win)
{

}

