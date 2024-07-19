#include "Input.h"
#include "Win.h"
#include "Util.h"

namespace {
	static JSClassID id;
	static JSClassDef inputClass = {
		.class_name{"Input"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto input = (Input*)JS_GetOpaque(val, id);
				delete input;
			}
		}
	};
}

Input::Input()
{
}

Input::~Input()
{
}

JSValue Input::constructor(JSContext* ctx, JSValueConst newTarget, int argc, JSValueConst* argv)
{
    static int tId{ WM_APP + 99 };
    tId += 1;
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Input();
    self->timerID = tId;
	JS_SetOpaque(obj, self);
	return obj;
}
void Input::Reg(JSContext* ctx)
{
	auto rt = JS_GetRuntime(ctx);
	JS_NewClassID(rt, &id);
	JS_NewClass(rt, id, &inputClass);
	JSValue protoInstance = JS_NewObject(ctx);
	RegBase(ctx, protoInstance);
    JS_SetPropertyStr(ctx, protoInstance, "setText", JS_NewCFunction(ctx, &Input::setText, "setText", 1));
    JS_SetPropertyStr(ctx, protoInstance, "setRect", JS_NewCFunction(ctx, &Input::setRect, "setRect", 1));
    JS_SetPropertyStr(ctx, protoInstance, "setPlaceHolder", JS_NewCFunction(ctx, &Input::setPlaceHolder, "setPlaceHolder", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseEnter", JS_NewCFunction(ctx, &Input::onMouseEnter, "onMouseEnter", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseLeave", JS_NewCFunction(ctx, &Input::onMouseLeave, "onMouseLeave", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseDown", JS_NewCFunction(ctx, &Input::onMouseDown, "onMouseDown", 1));
    JS_SetPropertyStr(ctx, protoInstance, "onMouseUp", JS_NewCFunction(ctx, &Input::onMouseUp, "onMouseUp", 1));

	JSValue ctroInstance = JS_NewCFunction2(ctx, &Input::constructor, inputClass.class_name, 0, JS_CFUNC_constructor, 0);
	JS_SetConstructor(ctx, ctroInstance, protoInstance);
	JS_SetClassProto(ctx, id, protoInstance);
	JSValue global = JS_GetGlobalObject(ctx);
	JS_SetPropertyStr(ctx, global, inputClass.class_name, ctroInstance);
	JS_FreeValue(ctx, global);
}

JSValue Input::setText(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->text = JS_DupValue(ctx, argv[0]);
    JS_SetPropertyStr(ctx, thisVal, "text", obj->text);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Input::setRect(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->rect = JS_DupValue(ctx, argv[0]);
    JS_SetPropertyStr(ctx, thisVal, "rect", obj->rect);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Input::setPlaceHolder(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->placeHolder = JS_DupValue(ctx, argv[0]);
    JS_SetPropertyStr(ctx, thisVal, "placeHolder", obj->placeHolder);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
void Input::Paint(Win* win)
{
    auto ctx = JS::GetCtx();
    auto rectObj = (Rect*)Element::GetPtr(rect);
    rectObj->Paint(win);
    Text* textObj = (Text*)Element::GetPtr(text);
    if (!textObj) {
        return;
    }
    Text* placeHolderObj = (Text*)Element::GetPtr(placeHolder);
    if (!placeHolderObj) {
        return;
    }
    win->canvas->save();
    win->canvas->clipRect(rectObj->rect);    
    if (textObj->text.empty() && !isFocus) {
        placeHolderObj->SetTextPos(rectObj->rect, placeHolderObj->lineRect);
        placeHolderObj->Paint(win);
    }
    else {
        textObj->SetTextPos(rectObj->rect, textObj->lineRect);
        textObj->Paint(win);
    }
    win->canvas->restore();
    if (showTextIbeam) {
        win->canvas->drawLine(textIbeamPos.fX, textObj->y + textObj->lineRect.fTop,
            textIbeamPos.fX, textIbeamPos.fY, paint);
    }
}
JSValue Input::onMouseEnter(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->mouseEnterCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Input::onMouseLeave(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->mouseLeaveCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Input::onMouseDown(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->mouseDownCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
JSValue Input::onMouseUp(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv)
{
    auto obj = (Input*)GetPtr(thisVal);
    obj->mouseUpCB = JS_DupValue(ctx, argv[0]);
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
void Input::MouseMove(const float& x, const float& y)
{
    auto rectObj = (Rect*)GetPtr(rect);
    auto flag = rectObj->rect.contains(x, y);
    if (flag == isMouseEnter) {
        return;
    }
    isMouseEnter = flag;
    auto ctx = JS::GetCtx();
    if (flag) {
        SetCursor(LoadCursor(NULL, IDC_IBEAM));
        if (!JS_IsFunction(ctx, mouseEnterCB)) {
            return;
        }
        JSValue ret = JS_Call(ctx, mouseEnterCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
        JS_FreeValue(ctx, ret);
    }
    else {
        HCURSOR cursor = (HCURSOR)GetCursor();
        if (cursor == LoadCursor(NULL, IDC_IBEAM)) {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
        }        
        if (!JS_IsFunction(ctx, mouseLeaveCB)) {
            return;
        }
        JSValue ret = JS_Call(ctx, mouseLeaveCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
        JS_FreeValue(ctx, ret);
    }
}
void Input::MouseDown(const float& x, const float& y)
{
    KillTimer(win->hwnd, timerID);
    isFocus = isMouseEnter;
    if (isFocus) {
        showTextIbeam = true;
        SetTimer(win->hwnd, timerID, 600, (TIMERPROC)nullptr);
        auto textObj = (Text*)Element::GetPtr(text);
        auto rectObj = (Rect*)Element::GetPtr(rect);
        textObj->SetTextPos(rectObj->rect, textObj->lineRect); //todo 只需要第一次这样就可以了，没必要每次mousedown都这样
        std::tie(textIndex, textIbeamPos) = textObj->getTextCursorPos(x);
        setImm();
        win->paint();
    }
}

void Input::MouseUp()
{
	//Rect::MouseUp();
}

void Input::CharInput(const unsigned int& val)
{
    auto textObj = (Text*)Element::GetPtr(text);
    std::wstring word{ (wchar_t)val };
    auto str1 = textObj->text.substr(0, textIndex);
    auto str2 = textObj->text.substr(textIndex);
    textObj->text = str1 + word + str2;
    textIndex += 1;
    textObj->resetLineRect();
    textIbeamPos = textObj->getTextCursorPosByWordIndex(textIndex);
    setImm();
}

void Input::Dispose()
{
    auto ctx = JS::GetCtx();
    JS_FreeValue(ctx, mouseEnterCB);
    JS_FreeValue(ctx, mouseLeaveCB);
    JS_FreeValue(ctx, mouseDownCB);
    JS_FreeValue(ctx, mouseUpCB);
}

void Input::Timeout(const unsigned int& id, Win* win)
{
    if (id != timerID) return;
    showTextIbeam = !showTextIbeam;
    win->paint();
}

void Input::setImm()
{
    if (HIMC himc = ImmGetContext(win->hwnd))
    {
        COMPOSITIONFORM comp = {};
        comp.ptCurrentPos.x = textIbeamPos.fX * win->scaleFactor;
        comp.ptCurrentPos.y = textIbeamPos.fY * win->scaleFactor;
        comp.dwStyle = CFS_FORCE_POSITION;
        ImmSetCompositionWindow(himc, &comp);
        CANDIDATEFORM cand = {};
        cand.dwStyle = CFS_CANDIDATEPOS;
        cand.ptCurrentPos.x = textIbeamPos.fX * win->scaleFactor;
        cand.ptCurrentPos.y = textIbeamPos.fY * win->scaleFactor;
        ImmSetCandidateWindow(himc, &cand);
        ImmReleaseContext(win->hwnd, himc);
    }
}

JSValue Input::GetChildById(const std::string& id)
{
    auto ctx = JS::GetCtx();
    if (!JS_IsUndefined(text)) {
        auto t = Element::GetPtr(text);
        if (t->idStr == id) {
            return JS_DupValue(ctx, text);
        }
    }
    else if (!JS_IsUndefined(placeHolder)) {
        auto t = Element::GetPtr(placeHolder);
        if (t->idStr == id) {
            return JS_DupValue(ctx, placeHolder);
        }
    }
    else {
        auto r = Element::GetPtr(rect);
        if (r->idStr == id) {
            return JS_DupValue(ctx, rect);
        }
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
