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
