#pragma once

#include "Div.h"
#include <chrono>

class Input: public Element
{
public:
	~Input();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
	void MouseMove(const float& x, const float& y) override;
	void MouseDown(const float& x, const float& y, Win* win) override;
	void MouseUp() override;
    void CharInput(const unsigned int& val);
	void Dispose() override;
    void Timeout(const unsigned int& id, Win* win) override;
    JSValue GetChildById(const std::string& id);
protected:
	Input();
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
    static JSValue setPlaceHolder(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    void setImm(const int& x, const int& y, Win* win);    
    JSValue text{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    JSValue placeHolder{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    JSValue rect{ JS::MakeVal(0, JS_TAG_UNDEFINED) };
    bool isFocus{ false };
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
    std::chrono::system_clock::time_point mouseDownTime;
    bool showTextIbeam{ false };
    int timerID{ 0 };
    float textIbeamPos{ 0.f };
    int textIndex{ 0 };
};
