#pragma once

#include "Element.h"

class TextBase:public Element
{
public:
    TextBase();
    ~TextBase();
protected:
    static void RegTextBase(JSContext* ctx, JSValue& protoInstance);
    virtual void resetTextRect() {};
private:    
    static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    double fontSize{ 13 };
    std::string fontFamily;
};


