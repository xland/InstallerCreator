#pragma once

#include "include/core/SkPaint.h"
extern "C" {
#include <quickjs.h>
}
#include "JS.h"

class Win;
class Element
{
public:
	~Element();
	virtual void Paint(Win* win) = 0;
	virtual void MouseMove(const float& x, const float& y) {};
	virtual void MouseDown(const float& x, const float& y) {};
    virtual void CharInput(const unsigned int& val) {};
	virtual void MouseUp() {};
	virtual void Dispose() {};
    virtual void Timeout(const unsigned int& id, Win* win) {};
	static Element* GetPtr(const JSValue& val);
	std::string idStr;
	bool visible{ true };
    Win* win;
protected:
	Element();
	static void RegBase(JSContext* ctx,JSValue& protoInstance);
	static std::tuple<double, double, double, double, JSValue> get4Arg(JSContext* ctx, JSValueConst* argv);
	SkPaint paint;
	float shadowSize{0};
	uint32_t shadowAmbientColor{ 0x33000000 };
	uint32_t shadowSpotColor{ 0x00000000 };
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
	static JSValue setId(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setVisible(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue getVisible(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStroke(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeWidth(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setLinearShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setRadialShader(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeCap(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setStrokeJoin(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setBlendMode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setShadowSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setShadowAmbientColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setShadowSpotColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};

