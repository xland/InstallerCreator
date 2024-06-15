#include "Img.h"
namespace {
	static JSClassID id;
	static JSClassDef divClass = {
		.class_name{"Img"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto div = (Img*)JS_GetOpaque(val, id);
				delete div;
			}
		}
	};
}

Img::Img()
{
}

Img::~Img()
{
}
JSValue Img::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Img();
	JS_SetOpaque(obj, self);
	return obj;
}
void Img::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &divClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegRectBase(ctx, protoInstance);
	//JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Div::setText, "setText", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setAlign", JS_NewCFunction(ctx, &Div::setAlign, "setAlign", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setTextColor", JS_NewCFunction(ctx, &Div::setTextColor, "setTextColor", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &Div::setFontSize, "setFontSize", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setFontFamily", JS_NewCFunction(ctx, &Div::setFontFamily, "setFontFamily", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "onMouseEnter", JS_NewCFunction(ctx, &Div::onMouseEnter, "onMouseEnter", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "onMouseLeave", JS_NewCFunction(ctx, &Div::onMouseLeave, "onMouseLeave", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "onMouseDown", JS_NewCFunction(ctx, &Div::onMouseDown, "onMouseDown", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "onMouseUp", JS_NewCFunction(ctx, &Div::onMouseUp, "onMouseUp", 1));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Img::constructor, divClass.class_name, 5, JS_CFUNC_constructor, 0);
	//JS_SetPropertyStr(ctx, ctroInstance, "newLTRB", JS_NewCFunction(ctx, &Div::newLTRB, "newLTRB", 4));
	//JS_SetPropertyStr(ctx, ctroInstance, "newXYWH", JS_NewCFunction(ctx, &Div::newXYWH, "newXYWH", 4));
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, divClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}