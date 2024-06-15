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

};

