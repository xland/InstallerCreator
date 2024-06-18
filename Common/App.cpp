#include <Windows.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <map>
#include <include/core/SkGraphics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkData.h>
#include "App.h"


namespace {
    JSValue app;
    std::map<std::string, SkFont*> fontMap;
}

App::App()
{
}

App::~App()
{
}
void App::Reg(JSContext* ctx)
{
    SkGraphics::Init();
    JSValue globalObj = JS_GetGlobalObject(ctx);
    app = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, app, "initFont", JS_NewCFunction(ctx, &App::initFont, "initFont", 1));
    JS_SetPropertyStr(ctx, app, "quit", JS_NewCFunction(ctx, &App::quit, "quit", 0));
    JS_SetPropertyStr(ctx, app, "setCursor", JS_NewCFunction(ctx, &App::setCursor, "setCursor", 1));
    JS_SetPropertyStr(ctx, app, "openUrlByDefaultBrowser", JS_NewCFunction(ctx, &App::openUrlByDefaultBrowser, "openUrlByDefaultBrowser", 1));
    JS_SetPropertyStr(ctx, globalObj, "app", app);
    JS_FreeValue(ctx, globalObj);
}
void App::Dispose() {
    auto ctx = JS::GetCtx();
    JS_FreeValue(ctx, app);
    for (const auto& pair : fontMap) {
        delete pair.second;
    }
    fontMap.clear();
}
SkFont* App::GetFont()
{
    return fontMap.begin()->second;
}
SkFont* App::GetFont(std::string& fontName)
{
    return fontMap[fontName];
}
JSValue App::quit(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    PostQuitMessage(0);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue App::setCursor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* strData = JS_ToCString(ctx, argv[0]);
    if (!strData) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    std::string cursorStr{strData};
    JS_FreeCString(ctx, strData);
    if (cursorStr == "pointer") {
        SetCursor(LoadCursor(NULL, IDC_HAND));
    }
    else if (cursorStr == "default") {
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue App::openUrlByDefaultBrowser(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* url = JS_ToCString(ctx, argv[0]);
    if (!url) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    HINSTANCE result = ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue App::initFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv) {
    if (argc < 1) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto fontMgr = SkFontMgr_New_GDI();
    uint32_t length = 0;
    JS_ToUint32(ctx, &length, JS_GetPropertyStr(ctx, argv[0], "length"));
    for (uint32_t i = 0; i < length; ++i) {
        auto fontNameVal = JS_GetPropertyUint32(ctx, argv[0], i);
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
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
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