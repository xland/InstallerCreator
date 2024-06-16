#include "Console.h"
#ifdef DEBUG
#include <iostream>
#include <fcntl.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#endif


Console::Console()
{
}

Console::~Console()
{
}

void Console::Reg(JSContext* ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, &Console::jsLog, "log", 1));
    JS_SetPropertyStr(ctx, console, "warn", JS_NewCFunction(ctx, &Console::jsLog, "warn", 1));
    JS_SetPropertyStr(ctx, console, "info", JS_NewCFunction(ctx, &Console::jsLog, "info", 1));
    JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, &Console::jsLog, "error", 1));
    JS_SetPropertyStr(ctx, globalObj, "console", console);
    JS_FreeValue(ctx, globalObj);
}

JSValue Console::jsLog(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv) {
    printf("[Console]:    ");
    for (int i = 0; i < argc; i++) {
        const char* str = JS_ToCString(ctx, argv[i]);
        if (str) {
            printf("  %s", str);
            JS_FreeCString(ctx, str);
        }
    }
    printf("\n");
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}