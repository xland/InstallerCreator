#include "TextBase.h"
#include "App.h"

TextBase::TextBase()
{
}

TextBase::~TextBase()
{
}

void TextBase::RegTextBase(JSContext* ctx, JSValue& protoInstance)
{
    JS_SetPropertyStr(ctx, protoInstance, "setFontSize", JS_NewCFunction(ctx, &TextBase::setFontSize, "setFontSize", 1));
    JS_SetPropertyStr(ctx, protoInstance, "setFontFamily", JS_NewCFunction(ctx, &TextBase::setFontFamily, "setFontFamily", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setPosition", JS_NewCFunction(ctx, &TextBase::setPosition, "setPosition", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setAlign", JS_NewCFunction(ctx, &TextBase::setAlign, "setAlign", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setIndent", JS_NewCFunction(ctx, &TextBase::setIndent, "setIndent", 2));
    JS_SetPropertyStr(ctx, protoInstance, "setPosition", JS_NewCFunction(ctx, &TextBase::setPosition, "setPosition", 2));
}

JSValue TextBase::setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    double fontSize;
    if (JS_ToFloat64(ctx, &fontSize, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto obj = (TextBase*)Element::GetPtr(thisVal);
    if (fontSize != obj->fontSize) {
        obj->fontSize = fontSize;
        obj->resetLineRect();
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue TextBase::setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* strData = JS_ToCString(ctx, argv[0]);
    if (!strData) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto ff = std::string{ strData };
    JS_FreeCString(ctx, strData);
    auto obj = (TextBase*)Element::GetPtr(thisVal);
    auto font = App::GetInitFont(ff);
    if (obj->font.get() != font.get()) {
        obj->font = font;
        obj->resetLineRect();
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue TextBase::setPosition(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    double x;
    if (JS_ToFloat64(ctx, &x, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    double y;
    if (JS_ToFloat64(ctx, &y, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    auto obj = (TextBase*)Element::GetPtr(thisVal);
    obj->x = x;
    obj->y = y;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

void TextBase::SetTextPos(SkRect& rect, SkRect& lineRect)
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
    x = left;
    y = top;
}

JSValue TextBase::setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    unsigned int vAlign;
    if (JS_ToUint32(ctx, &vAlign, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    unsigned int hAlign;
    if (JS_ToUint32(ctx, &hAlign, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    auto obj = (TextBase*)GetPtr(thisVal);
    obj->verticalAlign = vAlign;
    obj->horizontalAlign = hAlign;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue TextBase::setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (TextBase*)GetPtr(thisVal);
    double indentVertical;
    if (JS_ToFloat64(ctx, &indentVertical, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    double indentHorizontal;
    if (JS_ToFloat64(ctx, &indentHorizontal, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    obj->indentVertical = indentVertical;
    obj->indentHorizontal = indentHorizontal;
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
