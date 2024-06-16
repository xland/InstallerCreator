#pragma once
#include <include/core/SkFont.h>
#include "JS.h"

class App
{
public:
	App();
	~App();
	static void Reg(JSContext* ctx);
	static SkFont* GetFont(std::string& fontName);
	static SkFont* GetFont();
	static void Dispose();
private:
	static JSValue initFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue quit(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};

