#include <fstream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "include/core/SkGraphics.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/ports/SkTypeface_win.h"
#include "include/core/SkData.h"
#include "JS.h"
#include "Win.h"
#include "Paint.h"
#include "Path.h"
#include "Rect.h"
#ifdef DEBUG
#include <iostream>
#include <fcntl.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#endif

namespace {
    JSContext* ctx;
    JSRuntime* rt;
    std::map<std::string, SkFont*> fontMap;
}

void JS::Init()
{
    rt = JS_NewRuntime();
    if (!rt) return;
    ctx = JS_NewContext(rt);
    if (!ctx) {
        JS_FreeRuntime(rt);
        return;
    }
    regGlobal();
    Paint::Reg(ctx);
    Path::Reg(ctx);
    Rect::Reg(ctx);
    Win::Reg(ctx);
    loadIndexJs(ctx);
}

void JS::regGlobal()
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, &JS::jsLog, "log", 1));
    JS_SetPropertyStr(ctx, console, "warn", JS_NewCFunction(ctx, &JS::jsLog, "warn", 1));
    JS_SetPropertyStr(ctx, console, "info", JS_NewCFunction(ctx, &JS::jsLog, "info", 1));
    JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, &JS::jsLog, "error", 1));
    JS_SetPropertyStr(ctx, globalObj, "console", console);

    JSValue font = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, font, "init", JS_NewCFunction(ctx, &JS::initFont, "init", 1));
    JS_SetPropertyStr(ctx, globalObj, "font", font);

    JS_FreeValue(ctx, globalObj);
}

JSContext* JS::GetCtx()
{
    return ctx;
}
SkFont* JS::GetFont()
{    
    return fontMap.begin()->second;
}
SkFont* JS::GetFont(std::string& fontName)
{
    return fontMap[fontName];
}
JSValue JS::jsLog(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv) {
    printf("[Console]:    ");
    for (int i = 0; i < argc; i++) {
        const char* str = JS_ToCString(ctx, argv[i]);
        if (str) {
            printf("  %s", str);
            JS_FreeCString(ctx, str);
        }
    }
    printf("\n");
    return MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue JS::initFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv) {
    if (argc < 1) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto fontMgr = SkFontMgr_New_GDI();
    uint32_t length = 0;
    JS_ToUint32(ctx, &length, JS_GetPropertyStr(ctx, argv[0], "length"));
    for (uint32_t i = 0; i < length; ++i) {
        auto fontNameVal = JS_GetPropertyUint32(ctx, argv[0],i);
        const char* fontName = JS_ToCString(ctx, fontNameVal);
        if (!fontName) {
            return JS_ThrowTypeError(ctx, "arg arr item error");
        }
        std::string name{ fontName };
        JS_FreeCString(ctx, fontName);
        if (name.ends_with(".ttf")) {
            std::ifstream file(name, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                return JS_ThrowTypeError(ctx, "arg arr item error");
            }
            size_t fileSize = file.tellg();
            std::vector<uint8_t> buffer(fileSize);
            file.seekg(0, std::ios::beg);
            file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            file.close();
            auto fontData = SkData::MakeWithoutCopy(buffer.data(), fileSize);
            auto font = new SkFont(fontMgr->makeFromData(fontData));
            fontMap.insert({ name,font });
        }
        else {
            auto font = new SkFont(fontMgr->matchFamilyStyle(fontName, {}));
            fontMap.insert({ fontName,font });
        }
    }
    return MakeVal(0, JS_TAG_UNDEFINED);




//    SkString file_path;
//    file_path.printf("%s/%s", fResourceDir.c_str(), path.c_str());
//    std::unique_ptr<SkStreamAsset> file = SkFILEStream::Make(file_path.c_str());
//    if (!file) {
//        return false;
//    }
//#if defined(SK_TYPEFACE_FACTORY_FREETYPE)
//    sk_sp<SkTypeface> face =
//        SkTypeface_FreeType::MakeFromStream(std::move(file), SkFontArguments());
//#elif defined(SK_TYPEFACE_FACTORY_CORETEXT)
//    sk_sp<SkTypeface> face = SkTypeface_Mac::MakeFromStream(std::move(file), SkFontArguments());
//#elif defined(SK_TYPEFACE_FACTORY_DIRECTWRITE)
//    sk_sp<SkTypeface> face = DWriteFontTypeface::MakeFromStream(std::move(file), SkFontArguments());
//#else
//    sk_sp<SkTypeface> face = nullptr;
//#endif
//    if (familyName.empty()) {
//        fFontProvider->registerTypeface(std::move(face));
//    }
//    else {
//        fFontProvider->registerTypeface(std::move(face), SkString(familyName.c_str()));
//    }
}

void JS::loadIndexJs(JSContext* ctx)
{
    auto mainFilePath = "main.js";
    std::ifstream file(mainFilePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    JSValue val = JS_Eval(ctx, content.c_str(), content.size(), mainFilePath, JS_EVAL_TYPE_MODULE);
    if (JS_IsException(val)) {
        return;
    }
    JS_FreeValue(ctx, val);
}