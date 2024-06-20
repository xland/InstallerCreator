#include "Element.h"

class Text:public Element
{
public:
	Text();
	~Text();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
	std::wstring text;
	SkRect textRect;
	double fontSize{ 13 };
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};