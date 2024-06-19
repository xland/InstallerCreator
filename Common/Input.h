#include "Div.h"

class Input: public Div
{
public:
	~Input();
	static void Reg(JSContext* ctx);
	void Paint(Win* win) override;
protected:
	Input();
private:
	static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
	static JSValue newLTRB(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue newXYWH(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};
