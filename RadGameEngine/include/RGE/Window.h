#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

HWND CreateWnd(const CREATESTRUCT& cs, const class Window* wnd);

template<class T>
class WindowManager
{
public:
    static void GetWndClass(WNDCLASS& cs)
    {
        cs.lpszClassName = T::ClassName();
        T::GetWndClass(cs);
    }

    static ATOM Register()
    {
        WNDCLASS wc = {};
        GetWndClass(wc);
        return ::RegisterClass(&wc);
    }

    static void GetCreateWindow(CREATESTRUCT& cs)
    {
        cs.lpszClass = T::ClassName();
        T::GetCreateWindow(cs);
    }

    static T* Create(LPVOID lpCreateParams = nullptr)
    {
        CREATESTRUCT cs = {};
        cs.lpCreateParams = lpCreateParams;
        GetCreateWindow(cs);
        return Create(cs);
    }

    static T* Create(const CREATESTRUCT& cs)
    {
        T* self = new T();
        if (self && CreateWnd(cs, self) != NULL)
            return self;
        else
        {
            delete self;
            return nullptr;
        }
    }
};

class Window
{
public:
    HWND GetHWND() const { return m_hWnd; }
    operator HWND() const { return m_hWnd; }

protected:
    static void GetCreateWindow(CREATESTRUCT& cs);
    static void GetWndClass(WNDCLASS& wc);

    virtual ~Window() = default;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnDraw(const PAINTSTRUCT* pps) const { }

private:
    void OnPaint();
    void OnPrintClient(HDC hdc);

    static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hWnd = NULL;
};
