#pragma once

#include "Element.h"

class TextBase:public Element
{
public:
    TextBase();
    ~TextBase();
    std::tuple<float, float> GetTextPos(SkRect& rect, SkRect& lineRect);
    double x{ 0.0 }, y{ 0.0 };
    SkRect lineRect;
protected:
    static void RegTextBase(JSContext* ctx, JSValue& protoInstance);
    virtual void resetLineRect() {};
    std::shared_ptr<SkFont> font;
    double fontSize{ 13 };
    uint32_t verticalAlign;  //纵向
    uint32_t horizontalAlign; //横向
    double indentVertical{ 0.f };
    double indentHorizontal{ 0.f };
private:    
    static JSValue setFontSize(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setFontFamily(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setPosition(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setIndent(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
    static JSValue setAlign(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);    
};


