#pragma once
#include <string>
#include "include/core/SkFont.h"
#include "Rect.h"


class Div:public Rect
{
public:
	static void Reg(JSContext* ctx);
	~Div(); 
	void Paint(Win* win) override;
	void MouseMove(const float& x, const float& y) override;
	void MouseDown() override;
	void MouseUp() override;
	void Dispose() override;
private:
	Div();
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setTextColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

	static JSValue onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

	std::tuple<double, double> getTextPos(const SkRect& lineRect);

	JSValue mouseEnterCB;
	JSValue mouseLeaveCB;
	JSValue mouseDownCB;
	JSValue mouseUpCB;
	bool isMouseEnter{false};
	bool isMouseDown{ false };

	std::wstring  text;
	std::string iconStr;
	SkFont* font;
	float fontSize;
	unsigned int color;
	unsigned int verticalAlign;  //×ÝÏò
	unsigned int horizontalAlign; //ºáÏò
	bool isIcon{false};
};