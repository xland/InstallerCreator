#pragma once
#include "include/core/SkPaint.h"
#include "include/core/SkCanvas.h"
extern "C" {
#include <quickjs.h>
}
class Paint
{
public:
	static void Reg(JSContext* ctx);
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);

	static JSValue setStroke(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeWidth(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAntiAlias(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeCap(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeJoin(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setBlendMode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	
	static SkPaint* getPtr(JSValue& val);
private:
};

