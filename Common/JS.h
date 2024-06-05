#pragma once
extern "C" {
#include <quickjs.h>
#include <quickjs-libc.h>
}
class JS
{
public:
	static void Init();
	static constexpr JSCFunctionListEntry MakeJsFunc(const char* name, uint8_t length, JSCFunction* func);
	static constexpr JSValue MakeVal(int32_t val, int64_t tag);
private:
	static void regGlobal();
	static JSValue jsLog(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
	static void loadIndexJs(JSContext* ctx);
};
