#pragma once
#include "include/core/SkPath.h"
extern "C" {
#include <quickjs.h>
}
class Path
{
public:
	static void Reg(JSContext* ctx);
	~Path();
	static SkPath* getPtr(JSValue& val);
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue lineTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue moveTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue reset(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue addRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue addEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};

