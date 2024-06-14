#include "Win.h"
#include "include/utils/SkShadowUtils.h"
#include "include/core/SkPoint3.h"
#include "include/core/SkPaint.h"
#include "include/utils/SkTextUtils.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/ports/SkTypeface_win.h"
#include "include/core/SkData.h"
#include "Util.h"
#include "Rect.h"
#include "Path.h"
#include "JS.h"

void Win::regDraw(JSContext* ctx, JSValue& proto) {
    //JS_SetPropertyStr(ctx, proto, "drawRect", JS_NewCFunction(ctx, &Win::drawRect, "drawRect", 5));
    //JS_SetPropertyStr(ctx, proto, "drawEllipse", JS_NewCFunction(ctx, &Win::drawEllipse, "drawEllipse", 5));
    JS_SetPropertyStr(ctx, proto, "drawShadow", JS_NewCFunction(ctx, &Win::drawShadow, "drawShadow", 3));
    JS_SetPropertyStr(ctx, proto, "drawText", JS_NewCFunction(ctx, &Win::drawText, "drawText", 3));
}

//JSValue Win::drawRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
//{
//    auto win = getPtr(thisVal);
//    auto paint = Paint::getPtr(argv[0]);
//    auto rect = Rect::getPtr(argv[1]);
//    win->canvas->drawRect(*rect, *paint);
//    return JS::MakeVal(0, JS_TAG_UNDEFINED);
//}

//JSValue Win::drawEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
//{
//    auto win = getPtr(thisVal);
//    auto paint = Paint::getPtr(argv[0]);
//    auto rect = Rect::getPtr(argv[1]);
//    win->canvas->drawOval(*rect, *paint);
//    return JS::MakeVal(0, JS_TAG_UNDEFINED);
//}

JSValue Win::drawShadow(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto path = (Path*)Element::GetPtr(argv[0]);
    unsigned int ambientColor;
    if (JS_ToUint32(ctx, &ambientColor, argv[1])) {
        return JS_ThrowTypeError(ctx, "arg1 error");
    }
    unsigned int spotColor;
    if (JS_ToUint32(ctx, &spotColor, argv[2])) {
        return JS_ThrowTypeError(ctx, "arg2 error");
    }
    SkPoint3 zPlaneParams = SkPoint3::Make(0, 0, 30);// 定义阴影与 z 平面的关系    
    SkPoint3 lightPos = SkPoint3::Make(0, 0, 0);// 定义光源的位置和半径
    SkShadowUtils::DrawShadow(win->canvas.get(), path->path, zPlaneParams, lightPos, 60.f, ambientColor, spotColor, 0);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    //alignV left center right
    //alignH top middle bottom
    
    //auto win = getPtr(thisVal);
    //const char* strData = JS_ToCString(ctx, argv[0]);
    //if (!strData) {
    //    return JS_ThrowTypeError(ctx, "arg0 error");
    //}
    //auto str = Util::ConvertToWideChar(strData); 
    //JS_FreeCString(ctx, strData);
    //auto length = wcslen(str.data()) * 2;
    //auto font = JS::GetFont();
    //font->setSize(66);
    //SkRect lineRect;
    //font->measureText(str.data(), length, SkTextEncoding::kUTF16, &lineRect);
    //auto paint = Paint::getPtr(argv[1]);
    //SkTextUtils::Draw(win->canvas.get(), str.data(), length, SkTextEncoding::kUTF16, 100-lineRect.fLeft, 100-lineRect.fTop, *font, *paint, SkTextUtils::kLeft_Align);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}