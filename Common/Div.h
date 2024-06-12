#pragma once
#include <string>
#include "Rect.h"


class Div:public Rect
{
public:
	static void Reg(JSContext* ctx);
	~Div(); 
	void Paint(Win* win) override;
private:
	Div();
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	std::wstring  text;
	unsigned int verticalAlign;  //×ÝÏò
	unsigned int horizontalAlign; //ºáÏò
};