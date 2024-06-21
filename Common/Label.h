#include "Element.h"

class Label:public Element
{
public:
	~Label();	
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
private:
	Label();
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);

};