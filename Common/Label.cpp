#include "Label.h"
#include "Util.h"

namespace {
	static JSClassID id;
	static JSClassDef labelClass = {
		.class_name{"Label"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto text = (Label*)JS_GetOpaque(val, id);
				delete text;
			}
		}
	};
}

Label::Label()
{
}

Label::~Label()
{
}


void Label::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &labelClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	//JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &Label::setFontSize, "setFontSize", 1));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Label::constructor, labelClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, labelClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

JSValue Label::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Label();

	JS_SetOpaque(obj, self);
	return obj;
}
