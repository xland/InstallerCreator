#include "Win.h"
#include "JS.h"

namespace {
    static std::map<unsigned int, JSValue> timeoutCB;
    static std::map<unsigned int, JSValue> intervalCB;
}

void Win::timeoutProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    unsigned int id = idEvent;    
    auto ctx = JS::GetCtx();
    JSValue ret = JS_Call(ctx, timeoutCB[id], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
    JS_FreeValue(ctx, ret);
    JS_FreeValue(ctx,timeoutCB[id]);
    timeoutCB.erase(id);
}

void Win::intervalProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    unsigned int id = idEvent;
    auto ctx = JS::GetCtx();
    JSValue ret = JS_Call(ctx, intervalCB[id], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
    JS_FreeValue(ctx, ret);
}

void Win::regTimer(JSContext* ctx, JSValue& proto) {
    JS_SetPropertyStr(ctx, proto, "setTimeout", JS_NewCFunction(ctx, &Win::setTimeout, "setTimeout", 2));
    JS_SetPropertyStr(ctx, proto, "setInterval", JS_NewCFunction(ctx, &Win::setInterval, "setInterval", 2));
    JS_SetPropertyStr(ctx, proto, "clearTimeout", JS_NewCFunction(ctx, &Win::clearTimeout, "clearTimeout", 1));
    JS_SetPropertyStr(ctx, proto, "clearInterval", JS_NewCFunction(ctx, &Win::clearInterval, "clearInterval", 1));
}

JSValue Win::setTimeout(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int ms;
    if (JS_ToUint32(ctx, &ms, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    static unsigned int timerId = 0;
    timeoutCB.insert({ timerId,JS_DupValue(ctx, argv[0]) });
    SetTimer(win->hwnd, timerId, ms, (TIMERPROC)timeoutProc);
    auto ret = JS_NewUint32(ctx, timerId);
    timerId += 1;
    return ret;
}

JSValue Win::setInterval(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int ms;
    if (JS_ToUint32(ctx, &ms, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    static unsigned int timerId = 0;
    intervalCB.insert({ timerId,JS_DupValue(ctx, argv[0]) });
    SetTimer(win->hwnd, timerId, ms, (TIMERPROC)intervalProc);
    auto ret = JS_NewUint32(ctx, timerId);
    timerId += 1;
    return ret;
}

JSValue Win::clearTimeout(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int id;
    if (JS_ToUint32(ctx, &id, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    KillTimer(win->hwnd, id);
    JS_FreeValue(ctx, timeoutCB[id]);
    timeoutCB.erase(id);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::clearInterval(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int id;
    if (JS_ToUint32(ctx, &id, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    KillTimer(win->hwnd, id);
    JS_FreeValue(ctx, intervalCB[id]);
    intervalCB.erase(id);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}