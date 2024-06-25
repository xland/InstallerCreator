#pragma once

#include "TextBase.h"

class Text:public TextBase
{
public:
	~Text();
	static void Reg(JSContext* ctx);
    void Paint(Win* win) override;
    static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
    std::tuple<int, float> getTextCursorPos(const int& x);
    void resetLineRect() override;
    std::wstring text;
protected:
    Text();
private:
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    uint32_t decorationColor{ 0 };
    double decorationSize{ 0.f };
};
