#pragma once
#include "include/core/SkPath.h"
#include "Element.h"
extern "C" {
#include <quickjs.h>
}
class Path:public Element
{
public:
	static void Reg(JSContext* ctx);
	~Path();
	void Paint(Win* win) override;
	SkPath path;
protected:
	Path();
	static void RegPathBase(JSContext* ctx, JSValue& protoInstance);
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue lineTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue arcTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue moveTo(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue reset(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue addRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue addEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};

