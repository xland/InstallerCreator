#include <fstream>
#include <sstream>
#include <filesystem>
#include <fstream>
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
    JS_FreeValue(ctx, globalObj);
}

constexpr JSCFunctionListEntry JS::MakeJsFunc(const char* name, uint8_t length, JSCFunction* func)
{
    return JSCFunctionListEntry{
        .name{name},
        .prop_flags{ JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE },
        .def_type{JS_DEF_CFUNC},
        .magic{0},
        .u = {
            .func{
                .length{length},
                .cproto{JS_CFUNC_generic},
                .cfunc{
                    .generic{func}
                }
            }
        }
    };
}
constexpr JSValue JS::MakeVal(int32_t val, int64_t tag)
{
    return {
        .u{
            .int32{val}
        },
        .tag{tag}
    };
}
JSContext* JS::GetCtx()
{
    return ctx;
}
JSValue JS::jsLog(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
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