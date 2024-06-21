#include "Element.h"
#include "Rect.h"
#include "Text.h"

class Label:public Element
{
public:
	Label();
	~Label();	
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
};