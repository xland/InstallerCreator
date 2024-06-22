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
        obj->resetTextRect();
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
    if (obj->fontFamily != ff) {
        obj->fontFamily = ff;
        obj->resetTextRect();
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
