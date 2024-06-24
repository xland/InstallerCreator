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
    JSValue GetChildById(const std::string& id);
protected:
	Input();
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
    static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    JSValue text{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    JSValue rect{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    bool isMouseEnter{ false };
    bool isMouseDown{ false };
    JSValue mouseEnterCB;
    JSValue mouseLeaveCB;
    JSValue mouseDownCB;
    JSValue mouseUpCB;
    uint32_t verticalAlign;  //纵向
    uint32_t horizontalAlign; //横向
    double indentVertical{ 0.f };
    double indentHorizontal{ 0.f };
};
