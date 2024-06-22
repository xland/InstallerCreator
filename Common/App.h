#pragma once
#include <include/core/SkFont.h>
#include "JS.h"

class App
{
public:
	App();
	~App();
	static void Reg(JSContext* ctx);
	static std::shared_ptr<SkFont> GetSystemFont(const char* fontName);
	static std::shared_ptr<SkFont> GetFontByFile(const char* fontName);
	static std::shared_ptr<SkFont> GetDefaultTextFont();
	static std::shared_ptr<SkFont> GetDefaultIconFont();
	static void Dispose();
private:
    static JSValue initSystemFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue initFileFont(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

	static JSValue ready(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue quit(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue setCursor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue getKnownFolder(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
	static JSValue openUrlByDefaultBrowser(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

};

