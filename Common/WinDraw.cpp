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

    /*auto fFontCollection = sk_make_sp<skia::textlayout::FontCollection>();
    fFontCollection->setDefaultFontManager(SkFontMgr_New_GDI());
    skia::textlayout::TextStyle fTStyle;
    fTStyle.setFontFamilies({ SkString("Microsoft YaHei") });
    fTStyle.setColor(SK_ColorBLACK);
    const char* text =
        "中文问"
        "around and go to the next line. Sometimes, short sentence. Longer "
        "sentences are okay too because they are necessary. Very short. "
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
        "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
        "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
        "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
        "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
        "occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
        "mollit anim id est laborum. "
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
        "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
        "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
        "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
        "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
        "occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
        "mollit anim id est laborum.";
    skia::textlayout::ParagraphStyle paragraph_style;
    auto builder = skia::textlayout::ParagraphBuilder::make(paragraph_style, fFontCollection);
    builder->pushStyle(fTStyle);
    builder->addText(text);
    builder->pop();*/


    skia::textlayout::ParagraphStyle paragraph_style;
    paragraph_style.turnHintingOff(); // 可选，关闭提示提高渲染速度

    // 初始化TextStyle，指定字体支持中文
    skia::textlayout::TextStyle text_style;
    text_style.setFontFamilies({ SkString("Noto Sans CJK SC") }); // 一个支持中文的字体系列
    text_style.setColor(SK_ColorBLACK); // 文本颜色
    text_style.setFontSize(24); // 字体大小
    auto fFontCollection = sk_make_sp<skia::textlayout::FontCollection>();
    fFontCollection->setDefaultFontManager(SkFontMgr_New_GDI());
    // 创建ParagraphBuilder
    auto builder = skia::textlayout::ParagraphBuilder::make(paragraph_style, fFontCollection);
    builder->pushStyle(text_style);
    // 添加中文文本，UTF-8编码
    std::wstring wstr = L"你好，世界！";
    std::u16string chinese_text(wstr.begin(), wstr.end());
    builder->addText(chinese_text);

    // 构建Paragraph
    std::unique_ptr<skia::textlayout::Paragraph> paragraph = builder->Build();
    auto fParagraph = builder->Build();
    fParagraph->layout(300 - 20);
    fParagraph->paint(win->canvas.get(), 160, 160);
    
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}