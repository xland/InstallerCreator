#pragma once
#include <string>
#include "include/core/SkFont.h"
#include "Rect.h"


class Div: public Rect
{
public:
	static void Reg(JSContext* ctx);
	~Div(); 
	void Paint(Win* win) override;
	void MouseMove(const float& x, const float& y) override;
	void MouseDown() override;
	void MouseUp() override;
	void Dispose() override;
protected:
	Div();
	static void RegDivBase(JSContext* ctx, JSValue& protoInstance);
private:
	void paintIcon(Win* win);
	void piantText(Win* win);
	void paintDecoration(Win* win, float l, float r, float b);
	std::tuple<float,float,float,float> getTextPos(SkRect& lineRect);

	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue getText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue getIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setTextColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

	

	JSValue mouseEnterCB;
	JSValue mouseLeaveCB;
	JSValue mouseDownCB;
	JSValue mouseUpCB;
	bool isMouseEnter{false};
	bool isMouseDown{ false };

	std::wstring  text;
	uint32_t iconCode{0};
	SkFont* font;
	double fontSize{13};
	double indentVertical{0.f};
	double indentHorizontal{ 0.f };
	uint32_t decorationColor{ 0 };
	double decorationSize{ 0.f };
	unsigned int color;
	unsigned int verticalAlign;  //×ÝÏò
	unsigned int horizontalAlign; //ºáÏò
};