#pragma once

#include "Element.h"
#include "Rect.h"
#include "Text.h"

class Div:public Element
{
public:
    Div();
	~Div();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
    void Dispose() override;
    void MouseMove(const float& x, const float& y) override;
    void MouseDown() override;
    void MouseUp() override;
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);

    static JSValue onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

    static JSValue setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

    static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setIcon(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    //static JSValue setRRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

    std::tuple<float, float> getTextPos(SkRect& rect,SkRect& lineRect);
    JSValue mouseEnterCB;
    JSValue mouseLeaveCB;
    JSValue mouseDownCB;
    JSValue mouseUpCB;

    uint32_t verticalAlign;  //纵向
    uint32_t horizontalAlign; //横向
    double indentVertical{ 0.f };
    double indentHorizontal{ 0.f };
    JSValue text{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    JSValue rect{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    JSValue icon{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    bool isMouseEnter{ false };
    bool isMouseDown{ false };
};
