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
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "modules/skunicode/include/SkUnicode_icu.h"
#include "Util.h"
#include "Paint.h"
#include "Rect.h"
#include "Path.h"
#include "JS.h"

void Win::regDraw(JSContext* ctx, JSValue& proto) {
    JS_SetPropertyStr(ctx, proto, "fillColor", JS_NewCFunction(ctx, &Win::fillColor, "fillColor", 1));
    JS_SetPropertyStr(ctx, proto, "drawRect", JS_NewCFunction(ctx, &Win::drawRect, "drawRect", 5));
    JS_SetPropertyStr(ctx, proto, "drawEllipse", JS_NewCFunction(ctx, &Win::drawEllipse, "drawEllipse", 5));
    JS_SetPropertyStr(ctx, proto, "drawShadow", JS_NewCFunction(ctx, &Win::drawShadow, "drawShadow", 3));
    JS_SetPropertyStr(ctx, proto, "drawText", JS_NewCFunction(ctx, &Win::drawText, "drawText", 3));
}

JSValue Win::fillColor(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    unsigned int color;
    if (JS_ToUint32(ctx, &color, argv[0])) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    win->canvas->clear(color);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto paint = Paint::getPtr(argv[0]);
    auto rect = Rect::getPtr(argv[1]);
    win->canvas->drawRect(*rect, *paint);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawEllipse(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto paint = Paint::getPtr(argv[0]);
    auto rect = Rect::getPtr(argv[1]);
    win->canvas->drawOval(*rect, *paint);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawShadow(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto win = getPtr(thisVal);
    auto path = Path::getPtr(argv[0]);
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
    SkShadowUtils::DrawShadow(win->canvas.get(), *path, zPlaneParams, lightPos, 60.f, ambientColor, spotColor, 0);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}

JSValue Win::drawText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    //alignV left center right
    //alignH top middle bottom
    
    auto win = getPtr(thisVal);
    const char* strData = JS_ToCString(ctx, argv[0]);
    if (!strData) {
        return JS_ThrowTypeError(ctx, "arg0 error");
    }
    auto str = Util::ConvertToWideChar(strData); 
    JS_FreeCString(ctx, strData);
    auto length = wcslen(str.data()) * 2;
    auto font = JS::GetFont();
    auto paint = Paint::getPtr(argv[1]);
    //SkTextUtils::Draw(win->canvas.get(), str.data(), length, SkTextEncoding::kUTF16, 160, 160, *font, *paint, SkTextUtils::kLeft_Align);


    skia::textlayout::TextStyle style;
    style.setForegroundColor(*paint);
    style.setFontFamilies({ SkString("Microsoft YaHei") });
    style.setFontSize(10.5);
    skia::textlayout::ParagraphStyle paraStyle;
    paraStyle.setTextStyle(style);
    paraStyle.setTextAlign(skia::textlayout::TextAlign::kRight);

    sk_sp<SkUnicode> unicode = SkUnicodes::ICU::Make();
    if (!unicode) {
        printf("Could not load unicode data\n");
        return JS::MakeVal(0, JS_TAG_UNDEFINED);
    }

    auto fontMgr = SkFontMgr_New_GDI();
    auto fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
    fontCollection->setDefaultFontManager(fontMgr);

    using skia::textlayout::ParagraphBuilder;
    std::unique_ptr<ParagraphBuilder> builder = ParagraphBuilder::make(paraStyle, fontCollection, unicode);
    builder->addText(strData);

    std::unique_ptr<skia::textlayout::Paragraph> paragraph = builder->Build();
    paragraph->layout(800 - 20);
    paragraph->paint(win->canvas.get(), 10, 10);
    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}