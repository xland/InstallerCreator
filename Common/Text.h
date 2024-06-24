#pragma once

#include "TextBase.h"

class Text:public TextBase
{
public:
	~Text();
	static void Reg(JSContext* ctx);
    void Paint(Win* win) override;
    static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
    float getTextCursorPos(const int& x);
    std::wstring text;
protected:
    Text();
private:
	void resetLineRect() override;
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);


	
	double fontSize{ 13 };
    uint32_t decorationColor{ 0 };
    double decorationSize{ 0.f };
};
