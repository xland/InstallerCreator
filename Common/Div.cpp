#include "Div.h"
#include "Util.h"
#include "Win.h"
#include "Text.h"
#include "Rect.h"

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


void Div::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &divClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Div::constructor, divClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, divClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

void Div::Paint(Win* win)
{
}

JSValue Div::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
    auto text = Text::constructor(ctx, newTarget, argc, argv);
    auto rect = Rect::constructor(ctx, newTarget, argc, argv);
	JS_SetPropertyStr(ctx, obj, "text", text);
    JS_SetPropertyStr(ctx, obj, "rect", rect);
	auto self = new Div();
	JS_SetOpaque(obj, self);
	return obj;
}
