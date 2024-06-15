#include "Path.h"

class Shadow:public Path
{
public:
	~Shadow();
	void Paint(Win* win) override;
	static void Reg(JSContext* ctx);
protected:
	Shadow();
private:
	uint32_t ambientColor;
	uint32_t spotColor;
	float shadowSize{30.f};
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue setPath(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setAmbientColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setSpotColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setShadowSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
};

//shadow的path可能是窗口的轮廓,所以得考虑复用path的问题。
//假设path是一个临时变量的话，那么path就有可能被释放，
