#pragma once

#include "include/core/SkRect.h"
#include "include/core/SkRRect.h"
#include "Element.h"
class Rect:public Element
{
public:
	static void Reg(JSContext* ctx);
    static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	~Rect();
	void Paint(Win* win) override;
	SkRect rect;
protected:
	Rect();
	static void RegRectBase(JSContext* ctx, JSValue& protoInstance);
private:
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue contains(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setBorderRadius(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    void paintShadow(Win* win,const SkPath& path);
    bool isRRect{ false };
    double topLeft, topRight, bottomRight, bottomLeft;
};

