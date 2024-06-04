#include "JS.h"
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
}

void JS::regGlobal()
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, jsLog, "log", 1));
    JS_SetPropertyStr(ctx, console, "warn", JS_NewCFunction(ctx, jsLog, "warn", 1));
    JS_SetPropertyStr(ctx, console, "info", JS_NewCFunction(ctx, jsLog, "info", 1));
    JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, jsLog, "error", 1));
    JS_SetPropertyStr(ctx, globalObj, "console", console);
    JS_FreeValue(ctx, globalObj);
}

constexpr JSCFunctionListEntry MakeJsFunc(const char* name, uint8_t length, JSCFunction* func)
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
constexpr JSValue MakeVal(int32_t val, int64_t tag)
{
    return {
        .u{
            .int32{val}
        },
        .tag{tag}
    };
}
JSValue jsLog(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
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