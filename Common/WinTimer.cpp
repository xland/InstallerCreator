#include "Win.h"
#include "JS.h"

namespace {
    static std::map<unsigned int, JSValue> timeoutCB;
    static std::map<unsigned int, JSValue> intervalCB;
    unsigned int runningTimeoutCBId{ 0 };
    unsigned int runningIntervalCBId{ 0 };
    bool intervalCBUseless{ false };
}

void Win::timeoutProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    runningTimeoutCBId = idEvent;
    auto ctx = JS::GetCtx();
    JSValue ret = JS_Call(ctx, timeoutCB[runningTimeoutCBId], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
    JS_FreeValue(ctx, ret);
    JS_FreeValue(ctx,timeoutCB[runningTimeoutCBId]);
    timeoutCB.erase(runningTimeoutCBId);
    runningTimeoutCBId = 0;
}

void Win::intervalProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    runningIntervalCBId = idEvent;
    auto ctx = JS::GetCtx();
    JSValue ret = JS_Call(ctx, intervalCB[runningIntervalCBId], JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
    JS_FreeValue(ctx, ret);
    if (intervalCBUseless) {
        KillTimer(hWnd, runningIntervalCBId);
        JS_FreeValue(ctx, intervalCB[runningIntervalCBId]);
        intervalCB.erase(runningIntervalCBId);
    }
    runningIntervalCBId = 0;
    intervalCBUseless = false;
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
    static unsigned int timerId = 1;
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
    static unsigned int timerId = 1;
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
    if (id == runningTimeoutCBId) {
        return JS::MakeVal(0, JS_TAG_UNDEFINED);
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
    if (id == runningIntervalCBId) {
        intervalCBUseless = true;
    }
    else
    {
        KillTimer(win->hwnd, id);
        JS_FreeValue(ctx, intervalCB[id]);
        intervalCB.erase(id);
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

void Win::DisposeTimer()
{
    auto ctx = JS::GetCtx();
    for (const auto& [key, value] : timeoutCB) {
        KillTimer(hwnd, key);
        JS_FreeValue(ctx, value);
    }
    for (const auto& [key, value] : intervalCB) {
        KillTimer(hwnd, key);
        JS_FreeValue(ctx, value);
    }
}
