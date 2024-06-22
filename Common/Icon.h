#pragma once

#include "TextBase.h"

class Icon : public TextBase
{
public:
    Icon();
    ~Icon();
     static void Reg(JSContext* ctx);
     void Paint(Win* win) override;
private:
    void resetLineRect() override;
     static JSValue constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv);
     static JSValue setCode(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv);
     uint32_t iconCode{ 0 };
     std::string iconStr;
};
