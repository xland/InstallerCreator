#pragma once
#include "include/core/SkRect.h"
extern "C" {
#include <quickjs.h>
}
class Rect
{
public:
	static void Reg(JSContext* ctx);
	~Rect();
	static SkRect* getPtr(JSValue& val);
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue contains(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static std::tuple<double, double, double, double,JSValue> get4Arg(JSContext* ctx, JSValueConst* argv);
};

