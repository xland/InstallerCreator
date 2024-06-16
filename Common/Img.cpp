#include "Img.h"
#include "Win.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkStream.h"
#include "include/codec/SkCodec.h"

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
	JS_SetPropertyStr(ctx, protoInstance, "setImg", JS_NewCFunction(ctx, &Img::setImg, "setImg", 1));
	JS_SetPropertyStr(ctx, protoInstance, "setFillType", JS_NewCFunction(ctx, &Img::setFillType, "setFillType", 1));
	JSValue ctroInstance = JS_NewCFunction2(ctx, &Img::constructor, divClass.class_name, 5, JS_CFUNC_constructor, 0);
	JS_SetPropertyStr(ctx, ctroInstance, "newLTRB", JS_NewCFunction(ctx, &Img::newLTRB, "newLTRB", 4));
	JS_SetPropertyStr(ctx, ctroInstance, "newXYWH", JS_NewCFunction(ctx, &Img::newXYWH, "newXYWH", 4));
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, divClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}
void Img::Paint(Win* win)
{
	auto data = SkData::MakeFromFileName("img.png");
	auto img = SkImages::DeferredFromEncodedData(data);
	SkSamplingOptions option{ SkFilterMode::kLinear, SkMipmapMode::kLinear };
	win->canvas->drawImageRect(img, rect, option);
}

JSValue Img::setImg(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	return JSValue();
}

JSValue Img::setFillType(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	return JSValue();
}

JSValue Img::newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [l, t, r, b, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Img();
	self->rect.setLTRB(l, t, r, b);
	JS_SetOpaque(obj, self);
	return obj;
}

JSValue Img::newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
	auto [x, y, w, h, err] = get4Arg(ctx, argv);
	if (JS_IsException(err)) {
		return err;
	}
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Img();
	self->rect.setXYWH(x, y, w, h);
	JS_SetOpaque(obj, self);
	return obj;
}
