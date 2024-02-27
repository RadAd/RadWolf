#pragma once

#include <rpc.h>    // For gdiplus

#include "Window.h"
#include <bitset>
#include <vector>

#include <gdiplus.h>

inline bool GetKeyDown(int nVirtKey)
{
    return GetKeyState(nVirtKey) & KF_UP;
}

inline bool operator<(const LOGFONT& a, const LOGFONT& b)
{
    return memcmp(&a, &b, sizeof(LOGFONT)) < 0;
}

class GameWindow : public Window
{
    friend WindowManager<GameWindow>;
public:
    static ATOM Register() { return WindowManager<GameWindow>::Register(); }
    static GameWindow* Create(const LPCTSTR Title, int w, int h);
    static GameWindow* DoInit(const LPCTSTR lpCmdLine);

    void Close()
    {
        SendMessage(*this, WM_CLOSE, 0, 0);
    }

    float GetFPS() const;
    float GetMinFPS() const;
    float GetMaxFPS() const;
    Gdiplus::Size GetSize() const;
    bool IsMouseInWindow() const;
    Gdiplus::Point GetMousePos() const { return m_mouse; }

    bool GetKeyPressed(UINT vk) const
    {
        return m_KeyPressed.test(vk);
    }

protected:
    GameWindow()
    {
        s_gameWnd = this;
    }
    static void GetCreateWindow(CREATESTRUCT& cs);
    static void GetWndClass(WNDCLASS& wc);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    virtual void Init(const LPCTSTR lpCmdLine) = 0;
    virtual void Update(const DWORD ms) {}
    virtual void OnDraw(Gdiplus::Graphics* const g) const {}

private:
    static GameWindow* s_gameWnd;
    BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();
    void OnPaint();
    void OnKeyDown(UINT vk, int cRepeat, UINT flags);
    void OnLButtonDown(int x, int y, UINT keyFlags);
    void OnRButtonDown(int x, int y, UINT keyFlags);
    void OnMButtonDown(int x, int y, UINT keyFlags);
    void OnMouseMove(int x, int y, UINT keyFlags);
    void OnTimer(UINT id);

    static LPCTSTR ClassName() { return TEXT("RADGAMEENGINE"); }

    UINT m_DesiredElapsed = 0;
    DWORD m_Tick = 0;
    std::vector<DWORD> m_TickHist;
    std::bitset<256> m_KeyPressed;
    Gdiplus::Point m_mouse;
};
