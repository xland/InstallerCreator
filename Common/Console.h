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
	static void Log(const char* str);
private:
	static JSValue jsLog(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};