#pragma once
#include "Rect.h"
#include <string>
#include "include/core/SkImage.h"
class Img:public Rect
{
public:
	~Img();
	static void Reg(JSContext* ctx);
protected:
	Img();
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	void Paint(Win* win) override;
	static JSValue setSrc(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAlpha(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFillType(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	sk_sp<SkImage> img;
	unsigned alpha;
};

