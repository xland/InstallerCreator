#pragma once
#include "Rect.h"


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
	static JSValue setImg(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFillType(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};

