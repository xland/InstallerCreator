#pragma once
#include <Windows.h>
extern "C" {
#include <quickjs.h>
}
class Win
{
public:
	static void reg(JSContext* ctx);
private:
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
};

