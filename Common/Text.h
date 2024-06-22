#pragma once

#include "TextBase.h"

class Text:public TextBase
{
public:
	~Text();
	static void Reg(JSContext* ctx);
    void Paint(Win* win) override;
    static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);

    double x{ 0.0 }, y{ 0.0 };
    SkRect textRect;
protected:
    Text();
private:
	void resetTextRect();	
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setPosition(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setDecoration(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);


	std::wstring text;
	double fontSize{ 13 };
    uint32_t decorationColor{ 0 };
    double decorationSize{ 0.f };
	std::shared_ptr<SkFont> font;
};
