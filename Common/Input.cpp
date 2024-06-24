#include "Input.h"
#include "Win.h"

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
	JSValue obj = JS_NewObjectClass(ctx, id);
	auto self = new Input();
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

void Input::Paint(Win* win)
{
    auto ctx = JS::GetCtx();
    auto rectObj = (Rect*)Element::GetPtr(rect);
    rectObj->Paint(win);

    TextBase* textBase = (TextBase*)Element::GetPtr(text);
    if (!textBase) {
        return;
    }
    auto [left, top] = textBase->GetTextPos(rectObj->rect, textBase->lineRect);
    textBase->x = left;
    textBase->y = top;

    win->canvas->save();
    win->canvas->clipRect(rectObj->rect);
    textBase->Paint(win);
    win->canvas->restore();
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
        if (!JS_IsFunction(ctx, mouseEnterCB)) {
            return;
        }
        JSValue ret = JS_Call(ctx, mouseEnterCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
        JS_FreeValue(ctx, ret);
    }
    else {
        if (!JS_IsFunction(ctx, mouseLeaveCB)) {
            return;
        }
        JSValue ret = JS_Call(ctx, mouseLeaveCB, JS::MakeVal(0, JS_TAG_UNDEFINED), 0, nullptr);
        JS_FreeValue(ctx, ret);
    }
}

void Input::MouseDown(const float& x, const float& y, const Win* win)
{
    bool flag{ false };
    if (root->toolBar->hoverIndex == hoverIndexVal) {
        auto span = std::chrono::system_clock::now() - mouseDownTime;
        auto msCount = std::chrono::duration_cast<std::chrono::milliseconds>(span).count();
        if (msCount < 380) {
            //todo
            return;
        }
        mouseDownTime = std::chrono::system_clock::now();
        flag = true;
    }
    else {
        flag = false;
    }
    if (flag != isFocus) {
        isFocus = flag;
        showTextCursor = true;
        if (isFocus) {
            SetTimer(root->hwnd, timerID, 600, (TIMERPROC)nullptr);
            setImm(x, y);
        }
        else {
            KillTimer(root->hwnd, timerID);
            repaint();
        }
    }
    if (isFocus) {
        setTextCursorPos(x);
        repaint();
    }
}

void Input::MouseUp()
{
	//Rect::MouseUp();
}

void Input::Dispose()
{
	//Rect::Dispose();
}

void Input::setImm(const int& x, const int& y, const Win* win)
{
    if (HIMC himc = ImmGetContext(win->hwnd))
    {
        COMPOSITIONFORM comp = {};
        comp.ptCurrentPos.x = x;
        comp.ptCurrentPos.y = y;
        comp.dwStyle = CFS_FORCE_POSITION;
        ImmSetCompositionWindow(himc, &comp);
        CANDIDATEFORM cand = {};
        cand.dwStyle = CFS_CANDIDATEPOS;
        cand.ptCurrentPos.x = x;
        cand.ptCurrentPos.y = y;
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
    else {
        auto r = Element::GetPtr(rect);
        if (r->idStr == id) {
            return JS_DupValue(ctx, rect);
        }
    }
    return JS::MakeVal(0, JS_TAG_UNDEFINED);
}
