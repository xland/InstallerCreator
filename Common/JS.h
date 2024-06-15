#pragma once
#include "include/core/SkFont.h"
extern "C" {
#include <quickjs.h>
#include <quickjs-libc.h>
}
class JS
{
public:
	static void Init();
	static constexpr JSCFunctionListEntry MakeJsFunc(const char* name, uint8_t length, JSCFunction* func) {
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
	};
	static constexpr JSValue MakeVal(int32_t val, int64_t tag) {
		return {
			.u{
				.int32{val}
			},
			.tag{tag}
		};
	};
	static JSContext* GetCtx();
	static SkFont* GetFont(std::string& fontName);
	static SkFont* GetFont();
private:
	static void regGlobal();
	static JSValue jsLog(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue initFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue quit(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static void loadIndexJs(JSContext* ctx);	
};

