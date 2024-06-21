#include "Element.h"

class Text:public Element
{
public:
	~Text();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
    static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
private:
    Text();
	void resetTextRect();	
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontFile(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setPosition(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

	std::wstring text;
	SkRect textRect;
	double fontSize{ 13 };
	double x{ 0.0 }, y{ 0.0 };
	std::shared_ptr<SkFont> font;
};
