#pragma once

#include "Element.h"

class TextBase:public Element
{
public:
    TextBase();
    ~TextBase();
    double x{ 0.0 }, y{ 0.0 };
    SkRect lineRect;
protected:
    static void RegTextBase(JSContext* ctx, JSValue& protoInstance);
    virtual void resetLineRect() {};
    std::shared_ptr<SkFont> font;
    double fontSize{ 13 };
private:    
    static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setPosition(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);

    
};


