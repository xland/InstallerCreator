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

    JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Div::setText, "setText", 1));
    JS_SetPropertyStr(ctx, protoInstance, "setRect", JS_NewCFunction(ctx, &Div::setRect, "setRect", 1));
    //JS_SetPropertyStr(ctx, protoInstance, "setIcon", JS_NewCFunction(ctx, &Div::setIcon, "setIcon", 1));
    //JS_SetPropertyStr(ctx, protoInstance, "setRRect", JS_NewCFunction(ctx, &Div::setRRect, "setRRect", 1));

    JS_SetPropertyStr(ctx, protoInstance, "setAlign", JS_NewCFunction(ctx, &Div::setAlign, "setAlign", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setIndent", JS_NewCFunction(ctx, &Div::setIndent, "setIndent", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseEnter", JS_NewCFunction(ctx, &Div::onMouseEnter, "onMouseEnter", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseLeave", JS_NewCFunction(ctx, &Div::onMouseLeave, "onMouseLeave", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseDown", JS_NewCFunction(ctx, &Div::onMouseDown, "onMouseDown", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseUp", JS_NewCFunction(ctx, &Div::onMouseUp, "onMouseUp", 1));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Div::constructor, divClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);

	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, divClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

void Div::Paint(Win* win)
{
    auto ctx = JS::GetCtx();
    auto rectObj = (Rect*)Element::GetPtr(rect);
    auto txtObj = (Text*)Element::GetPtr(text);
    auto [left, top] = getTextPos(rectObj->rect, txtObj->lineRect);
    txtObj->x = left;
    txtObj->y = top;
    rectObj->Paint(win);
    txtObj->Paint(win);
}

JSValue Div::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
	JSValue obj = JS_NewObjectClass(ctx, id);
    auto self = new Div();
	JS_SetOpaque(obj, self);
	return obj;
}

JSValue Div::setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->text = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::setRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->rect = JS_DupValue(ctx, argv[0]);
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
    auto div = (Div*)GetPtr(thisVal);
    div->verticalAlign = vAlign;
    div->horizontalAlign = hAlign;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

std::tuple<float, float> Div::getTextPos(SkRect& rect, SkRect& lineRect)
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
    return {left,top};
}

JSValue Div::setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
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

JSValue Div::onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->mouseEnterCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->mouseLeaveCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->mouseDownCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Div::onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto div = (Div*)GetPtr(thisVal);
    div->mouseUpCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
