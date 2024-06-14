#pragma once

#include "include/core/SkPaint.h"
extern "C" {
#include <quickjs.h>
}
#include "JS.h"

class Win;
class Element
{
public:
	~Element();
	virtual void Paint(Win* win) = 0;
	virtual void MouseMove(const float& x, const float& y) {};
	static Element* GetPtr(JSValue& val);
protected:
	Element();
	static void RegBase(JSContext* ctx,JSValue& protoInstance);
	static std::tuple<double, double, double, double, JSValue> get4Arg(JSContext* ctx, JSValueConst* argv);
	SkPaint paint;
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue setStroke(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeWidth(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setLinearShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setRadialShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAntiAlias(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeCap(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeJoin(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setBlendMode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};

