#include "Icon.h"
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/core/SkFontTypes.h>
#include <include/ports/SkTypeface_win.h>
#include "App.h"
#include "Win.h"

namespace {
	static JSClassID id;
	static JSClassDef iconClass = {
		.class_name{"Icon"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto icon = (Icon*)JS_GetOpaque(val, id);
				delete icon;
			}
		}
	};
}

Icon::Icon()
{
}

Icon::~Icon()
{
}

void Icon::Reg(JSContext* ctx)
{
    auto rt = JS_GetRuntime(ctx);
    JS_NewClassID(rt, &id);
    JS_NewClass(rt, id, &iconClass);
    JSValue protoInstance = JS_NewObject(ctx);
    Element::RegBase(ctx, protoInstance);
    TextBase::RegTextBase(ctx, protoInstance);
    JS_SetPropertyStr(ctx, protoInstance, "setIcon", JS_NewCFunction(ctx, &Icon::setIcon, "setIcon", 1));
    JS_SetPropertyStr(ctx, protoInstance, "getIcon", JS_NewCFunction(ctx, &Icon::getIcon, "getIcon", 0));
    JSValue ctroInstance = JS_NewCFunction2(ctx, &Icon::constructor, iconClass.class_name, 0, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, ctroInstance, protoInstance);
    JS_SetClassProto(ctx, id, protoInstance);
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, iconClass.class_name, ctroInstance);
    JS_FreeValue(ctx, global);
}

void Icon::Paint(Win* win)
{
    win->canvas->drawSimpleText(iconStr.c_str(), 4, SkTextEncoding::kUTF8, x, y, *font.get(), paint);
}

JSValue Icon::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
    JSValue obj = JS_NewObjectClass(ctx, id);
    auto self = new Icon();
    JS_SetOpaque(obj, self);
    return obj;
}

JSValue Icon::setIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto icon = (Icon*)GetPtr(thisVal);
    uint32_t iconCode;
    if (JS_ToUint32(ctx, &iconCode, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    if (icon->iconCode != iconCode) {        
        icon->iconCode = iconCode;
        std::u8string utf8Str;
        utf8Str += static_cast<char8_t>((iconCode >> 12) | 0xE0);
        utf8Str += static_cast<char8_t>(((iconCode >> 6) & 0x3F) | 0x80);
        utf8Str += static_cast<char8_t>((iconCode & 0x3F) | 0x80);
        icon->iconStr = std::string(utf8Str.begin(), utf8Str.end());
        icon->resetLineRect();
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Icon::getIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto icon = (Icon*)GetPtr(thisVal);
    return JS_NewUint32(ctx, icon->iconCode);
}
void Icon::resetLineRect()
{
    if (!font.get()) {
        font = App::GetDefaultIconFont();
    }
    font->setSize(fontSize);
    font->measureText(iconStr.c_str(), 4, SkTextEncoding::kUTF8, &lineRect);
}
