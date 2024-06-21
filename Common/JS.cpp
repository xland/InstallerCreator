#include <fstream>
#include <sstream>
#include <filesystem>
#include <fstream>

#include "JS.h"
#include "App.h"
#include "Console.h"
#include "Win.h"
#include "Path.h"
#include "Rect.h"
#include "Text.h"
#include "Div.h"
#include "Input.h"
#include "Img.h"
#include "Shadow.h"

namespace {
    JSContext* ctx;
    JSRuntime* rt;
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
    App::Reg(ctx);
    Console::Reg(ctx);
    Shadow::Reg(ctx);
    Path::Reg(ctx);
    Rect::Reg(ctx);
    Text::Reg(ctx);
    Div::Reg(ctx);
    Input::Reg(ctx);
    Win::Reg(ctx);
    Img::Reg(ctx);
    loadIndexJs(ctx);
}

JSContext* JS::GetCtx()
{
    return ctx;
}

JSRuntime* JS::GetRt()
{
    return rt;
}

void JS::Dispose()
{
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}

void JS::loadIndexJs(JSContext* ctx)
{
    auto mainFilePath = "main.js";
    if (!std::filesystem::exists(mainFilePath)) {
        MessageBox(NULL, L"Can not finde main.js.", L"Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
        return;
    }
    std::ifstream file(mainFilePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    JSValue val = JS_Eval(ctx, content.c_str(), content.size(), mainFilePath, JS_EVAL_TYPE_MODULE);
    if (JS_IsException(val)) {
        Console::Err(val);
        return;
    }
    JS_FreeValue(ctx, val);
}