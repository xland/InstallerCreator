#include "Win.h"
#include <windowsx.h>
#include "Element.h"

LRESULT CALLBACK Win::RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        LPVOID pThis = pCS->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    auto winPtr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    auto obj = reinterpret_cast<Win*>(winPtr);
    if (obj)
    {
        switch (msg)
        {
        case WM_NCCALCSIZE:
        {
            return false;
        }
        case WM_SETCURSOR:
        {
            return true;
        }
        case WM_LBUTTONDOWN:
        {
            obj->lBtnDown(lParam);
            break;
        }
        case WM_LBUTTONUP:
        {
            obj->lBtnUp();
            break;
        }
        case WM_TIMER: {
            obj->onTimeout(wParam);
            break;
        }
        case WM_MOUSEMOVE:
        {
            obj->mouseMove(lParam);
            break;
        }
        case WM_MOUSELEAVE: {
            obj->mouseLeave();
            return true;
        }
        case WM_CLOSE: {
            obj->closed();
            //PostQuitMessage(0);
            return true;
            break;
        }
        case WM_MOUSEWHEEL: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            //obj->mouseWheel(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
            break;
        }
        case WM_CHAR: {
            obj->onChar(wParam);
            break;
        }
        case WM_DPICHANGED: {
            obj->dpiChanged(wParam, lParam);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Win::lBtnDown(const LPARAM& lParam)
{
    auto x = (float)GET_X_LPARAM(lParam) / scaleFactor;
    auto y = (float)GET_Y_LPARAM(lParam) / scaleFactor;
    if (captionPath.contains(x, y)) {
        isCaptionMouseDown = true;
        GetCursorPos(&startPos);
        SetCapture(hwnd);
    }
    else {
        int i = elements.size() - 1;
        auto a = Element::GetPtr(elements[0]);
        if (i < 0) return;
        for (; i >= 0; i--) {
            if (i == 0) {
               auto a = 1;
            }
            auto element = Element::GetPtr(elements[i]);
            if (!element) continue;
            element->MouseDown(x,y);
        }
    }
}

void Win::lBtnUp()
{
    isCaptionMouseDown = false;
    int i = elements.size() - 1;
    if (i < 0) return;
    for (; i >= 0; i--) {
        auto element = Element::GetPtr(elements[i]);
        element->MouseUp();
    }
}

void Win::mouseMove(const LPARAM& lParam)
{
    if (!isTrackMouseEvent) {
        TRACKMOUSEEVENT tme = {};
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = hwnd;
        tme.dwHoverTime = 1;
        isTrackMouseEvent = TrackMouseEvent(&tme);
    }
    if (isCaptionMouseDown) {
        POINT point;
        GetCursorPos(&point);
        int dx = point.x - startPos.x;
        int dy = point.y - startPos.y;
        x += dx;
        y += dy;
        SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
        startPos = point;
    }
    else {
        auto x = (float)GET_X_LPARAM(lParam) / scaleFactor;
        auto y = (float)GET_Y_LPARAM(lParam) / scaleFactor;
        int i = elements.size() - 1;
        if (i < 0) return;
        for (; i >= 0; i--) {
            auto element = Element::GetPtr(elements[i]);
            element->MouseMove(x, y);
        }
    }
}

void Win::mouseLeave()
{
    TRACKMOUSEEVENT tme = {};
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_CANCEL | TME_HOVER | TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);
    isTrackMouseEvent = false;
    for (auto& obj : elements)
    {
        auto element = Element::GetPtr(obj);
        element->MouseMove(-888888, -888888);
    }
}

void Win::dpiChanged(const WPARAM& wParam, const LPARAM& lParam)
{
    UINT dpi = LOWORD(wParam);
    scaleFactor = static_cast<float>(dpi) / 96.0f;
    canvas->scale(scaleFactor, scaleFactor);
    RECT* rect = (RECT*)lParam;
    x = rect->left;
    y = rect->top;
    w = rect->right - rect->left;
    h = rect->bottom - rect->top;
    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
    DeleteDC(hCompatibleDC);
    DeleteObject(bottomHbitmap);
    initCanvas();
    paint();

    //if (!captionPath.isEmpty()) {
    //    SkMatrix matrix;
    //    matrix.setScale(scaleFactor, scaleFactor);
    //    captionPath.transform(matrix, &captionPath);
    //}

}

void Win::onTimeout(const unsigned int& id)
{
    for (auto& obj : elements)
    {
        auto element = Element::GetPtr(obj);
        element->Timeout(id, this);
    }
}

void Win::onChar(const unsigned int& val)
{
    for (auto& obj : elements)
    {
        auto element = Element::GetPtr(obj);
        element->CharInput(val);
    }
}
