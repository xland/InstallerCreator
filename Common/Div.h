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
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
};
