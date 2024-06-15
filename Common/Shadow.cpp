#include "Shadow.h"
#include "Win.h"
#include "Path.h"
#include "include/core/SkPoint3.h"
#include "include/utils/SkShadowUtils.h"

namespace {
	static JSClassID id;
	static JSClassDef shadowClass = {
		.class_name{"Shadow"},
		.finalizer{[](JSRuntime* rt, JSValue val) {	
				auto shadow = (Shadow*)JS_GetOpaque(val, id);
				delete shadow;
			}
		}
	};
}

Shadow::Shadow()
{
}

Shadow::~Shadow()
{
}

void Shadow::Paint(Win* win)
{
	SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, 30);// 定义阴影与 z 平面的关系    
	SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
	SkShadowUtils::DrawShadow(win->canvas.get(), path, zPlaneParams, lightPos, 60.f, ambientColor, spotColor, 0);
}

JSValue Shadow::constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Shadow();
	JS_SetOpaque(obj, self);
	return obj;
}
JSValue Shadow::setPath(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto shadow = (Shadow*)JS_GetOpaque(thisVal, id);
	auto path = (Path*)Element::GetPtr(argv[0]);
	shadow->path = path->path; //这里是个复制操作
	return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
void Shadow::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &shadowClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
	RegPathBase(ctx, protoInstance);
	JS_SetPropertyStr(ctx, protoInstance, "setPath", JS_NewCFunction(ctx, &Shadow::setPath, "setPath", 1));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Shadow::constructor, shadowClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, shadowClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}
