#pragma once
#include "JS.h"
extern "C" {
#include <quickjs.h>
}


class Console
{
public:
	static void Reg(JSContext* ctx);
	Console();
	~Console();

private:
	static JSValue jsLog(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};