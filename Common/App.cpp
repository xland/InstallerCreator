#include <Windows.h>
#include <shlobj_core.h>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <map>
#include <include/core/SkGraphics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkData.h>
#include "Util.h"
#include "App.h"
#include "Console.h"


namespace {
    JSValue app;
    std::map<std::string,std::shared_ptr<SkFont>> fonts;
    std::string defaultTextFontName;
    std::string defaultIconFontName;
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
    JS_SetPropertyStr(ctx, app, "ready", JS_NewCFunction(ctx, &App::ready, "ready", 1));
    JS_SetPropertyStr(ctx, app, "initSystemFont", JS_NewCFunction(ctx, &App::initSystemFont, "initFont", 4));
    JS_SetPropertyStr(ctx, app, "initFileFont", JS_NewCFunction(ctx, &App::initFileFont, "initFont", 4));
    JS_SetPropertyStr(ctx, app, "quit", JS_NewCFunction(ctx, &App::quit, "quit", 0));
    JS_SetPropertyStr(ctx, app, "setCursor", JS_NewCFunction(ctx, &App::setCursor, "setCursor", 1));
    JS_SetPropertyStr(ctx, app, "getKnownFolder", JS_NewCFunction(ctx, &App::getKnownFolder, "getKnownFolder", 1));
    JS_SetPropertyStr(ctx, app, "openUrlByDefaultBrowser", JS_NewCFunction(ctx, &App::openUrlByDefaultBrowser, "openUrlByDefaultBrowser", 1));
    JS_SetPropertyStr(ctx, globalObj, "app", app);
    JS_FreeValue(ctx, globalObj);
}
void App::Dispose() {
    auto ctx = JS::GetCtx();
    JS_FreeValue(ctx, app);
}

std::shared_ptr<SkFont> App::GetSystemFont(const char* fontName)
{
    auto fontMgr = SkFontMgr_New_GDI();
    SkFontStyle fontStyle = SkFontStyle::Normal();
    auto font = std::make_shared<SkFont>(fontMgr->matchFamilyStyle(fontName, fontStyle));
    font->setSize(13);
    font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
    font->setSubpixel(true);
    return font;
}

std::shared_ptr<SkFont> App::GetFontByFile(const char* fontName)
{
    std::ifstream file(fontName, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Console::Err("can not open font file!");
        return nullptr;
    }
    size_t fileSize = file.tellg();
    std::vector<uint8_t> buffer(fileSize);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    auto fontData = SkData::MakeWithoutCopy(buffer.data(), fileSize);
    auto fontMgr = SkFontMgr_New_GDI();
    auto font = std::make_shared<SkFont>(fontMgr->makeFromData(fontData));
    font->setSize(13);
    font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
    font->setSubpixel(true);
    return font;
}

std::shared_ptr<SkFont> App::GetDefaultTextFont()
{
    return fonts[defaultTextFontName];
}

std::shared_ptr<SkFont> App::GetDefaultIconFont()
{
    return fonts[defaultIconFontName];
}

JSValue App::ready(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    if (!JS_IsFunction(ctx, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    if (defaultTextFontName.empty()) {
        defaultTextFontName = std::string{ "Microsoft YaHei" };
        auto font = GetSystemFont(defaultTextFontName.data());
        fonts.insert({ defaultTextFontName, font });
    }
    JSValue ret = JS_Call(ctx, argv[0], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
    JS_FreeValue(ctx, ret);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
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
JSValue App::getKnownFolder(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    //https://learn.microsoft.com/zh-tw/windows/win32/shell/knownfolderid
    const char* guid = JS_ToCString(ctx, argv[0]);
    if (!guid) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto id = Util::ConvertToGuid(guid);
    JS_FreeCString(ctx, guid);
    PWSTR pszPath;
    HRESULT hr = SHGetKnownFolderPath(id, 0, NULL, &pszPath);
    std::wstring pathStr(pszPath);
    CoTaskMemFree(pszPath);
    auto str = Util::ConvertToStr(pathStr);
    return JS_NewString(ctx, str.data());
}
JSValue App::openUrlByDefaultBrowser(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* url = JS_ToCString(ctx, argv[0]);
    if (!url) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    HINSTANCE result = ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    JS_FreeCString(ctx, url);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue App::initSystemFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* name = JS_ToCString(ctx, argv[0]);
    if (!name) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto font = GetSystemFont(name);
    std::string fontName{ name };
    JS_FreeCString(ctx, name);

    bool isDefaultTextFont = JS_ToBool(ctx, argv[1]);
    if (isDefaultTextFont) {
        defaultTextFontName = fontName;
    }
    fonts.insert({ fontName, font });
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue App::initFileFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    const char* fontFilePath = JS_ToCString(ctx, argv[0]);
    if (!fontFilePath) {
        return JS_ThrowTypeError(ctx, "arg0 arr item error");
    }

    const char* name = JS_ToCString(ctx, argv[1]);
    if (!name) {
        return JS_ThrowTypeError(ctx, "arg1 arr item error");
    }

    uint32_t val;
    if (JS_ToUint32(ctx, &val, argv[2])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    std::string fontName { name };
    if (val == 1) {
        defaultIconFontName = fontName;
    }
    else if (val == 2) {
        defaultTextFontName = fontName;
    }
    auto font = GetFontByFile(fontFilePath);
    fonts.insert({fontName,font});

    JS_FreeCString(ctx, fontFilePath);
    JS_FreeCString(ctx, name);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
