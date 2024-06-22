#pragma once

#include "Div.h"

class Input: public Element
{
public:
	~Input();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
	void MouseMove(const float& x, const float& y) override;
	void MouseDown() override;
	void MouseUp() override;
	void Dispose() override;
protected:
	Input();
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};
