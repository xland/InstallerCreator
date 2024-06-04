#include "Win.h"
#include <map>

namespace {
	static JSClassID id{ 0 };
	static JSClassDef jsWinClass = {
	.class_name{"Win"}, // ¿‡√˚
	.finalizer{[](JSRuntime* rt, JSValue val) {} }
	};
}


JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);

	JS_SetPropertyStr(ctx, obj, "id", JS_NewUint32(ctx, winId));
	JS_SetPropertyStr(ctx, obj, "__cbs", JS_NewArray(ctx));
	wins.insert({ winId,obj });
	return obj;
}

void Win::reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &jsWinClass);
	JSValue protoInstance = JS_NewObject(ctx);
	//JS_SetPropertyStr(ctx, protoInstance, "show", JS_NewCFunction(ctx, show, "show", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "showBrowser", JS_NewCFunction(ctx, showBrowser, "showBrowser", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setKiosk", JS_NewCFunction(ctx, setKiosk, "setKiosk", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setPublic", JS_NewCFunction(ctx, setPublic, "setPublic", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "getUrl", JS_NewCFunction(ctx, getUrl, "getUrl", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "setSize", JS_NewCFunction(ctx, setSize, "setSize", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setIcon", JS_NewCFunction(ctx, setIcon, "setIcon", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setProfile", JS_NewCFunction(ctx, setProfile, "setProfile", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setProxy", JS_NewCFunction(ctx, setProxy, "setProxy", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "setPosition", JS_NewCFunction(ctx, setPosition, "setPosition", 2));
	//JS_SetPropertyStr(ctx, protoInstance, "setRootFolder", JS_NewCFunction(ctx, setRootFolder, "setRootFolder", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "navigate", JS_NewCFunction(ctx, navigate, "navigate", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "close", JS_NewCFunction(ctx, close, "close", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "destroy", JS_NewCFunction(ctx, destroy, "destroy", 0));
	//JS_SetPropertyStr(ctx, protoInstance, "run", JS_NewCFunction(ctx, run, "run", 1));
	//JS_SetPropertyStr(ctx, protoInstance, "bind", JS_NewCFunction(ctx, bind, "bind", 2));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Win::constructor, jsWinClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, jsWinClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}