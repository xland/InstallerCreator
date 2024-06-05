#include "JS.h"
#include "Win.h"
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
    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
    js_std_init_handlers(rt);
    ctx = JS_NewContext(rt);
    if (!ctx) {
        JS_FreeRuntime(rt);
        return;
    }
    js_init_module_std(ctx, "std");
    js_init_module_os(ctx, "os");
    regGlobal();
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
    size_t bufLen;
    uint8_t* buf = js_load_file(ctx, &bufLen, mainFilePath);
    if (!buf) {
        perror("load file error:");
        perror(mainFilePath);
    }
    char* bufStr = reinterpret_cast<char*>(const_cast<uint8_t*>(buf));
    JSValue val = JS_Eval(ctx, bufStr, bufLen, mainFilePath, JS_EVAL_TYPE_MODULE);
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        return;
    }
    JS_FreeValue(ctx, val);
    js_free(ctx, buf);
}