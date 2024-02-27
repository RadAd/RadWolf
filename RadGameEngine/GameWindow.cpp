#include "include/RGE/GameWindow.h"
#include "Windowxx.h"
#include <tchar.h>
#include <algorithm>
#include <numeric>

GameWindow* GameWindow::s_gameWnd = nullptr;

GameWindow* GameWindow::Create(const LPCTSTR Title, int w, int h)
{
    //return WindowManager<GameWindow>::Create();
    CREATESTRUCT cs = {};
    WindowManager<GameWindow>::GetCreateWindow(cs);
    RECT rectScreen;
    GetClientRect(GetDesktopWindow(), &rectScreen);
    cs.lpszName = Title;
    RECT r = { 0, 0, w, h };
    OffsetRect(&r, (rectScreen.right + rectScreen.left - w) / 2, (rectScreen.bottom + rectScreen.top - h) / 2);
    AdjustWindowRectEx(&r, cs.style, cs.hMenu != NULL, cs.dwExStyle);
    cs.x = r.left;
    cs.y = r.top;
    cs.cx = r.right - r.left;
    cs.cy = r.bottom - r.top;
    //return WindowManager<GameWindow>::Create(cs);
    if (s_gameWnd != nullptr && CreateWnd(cs, s_gameWnd) != NULL)
        return s_gameWnd;
    else
        return nullptr;
}

GameWindow* GameWindow::DoInit(const LPCTSTR lpCmdLine)
{
    if (s_gameWnd != nullptr)
        s_gameWnd->Init(lpCmdLine);
    return s_gameWnd;
}

float GameWindow::GetFPS() const
{
    DWORD TickTotal = std::accumulate(m_TickHist.begin(), m_TickHist.end(), 0);
    return m_TickHist.size() / (TickTotal / 1000.0f);
}

float GameWindow::GetMinFPS() const
{
    if (m_TickHist.empty())
        return 0;
    DWORD TickTotal = *std::max_element(m_TickHist.begin(), m_TickHist.end());
    return 1 / (TickTotal / 1000.0f);
}

float GameWindow::GetMaxFPS() const
{
    if (m_TickHist.empty())
        return 0;
    DWORD TickTotal = *std::min_element(m_TickHist.begin(), m_TickHist.end());
    return 1 / (TickTotal / 1000.0f);
}

Gdiplus::Size GameWindow::GetSize() const
{
    RECT r;
    GetClientRect(*this, &r);
    Gdiplus::Rect rp(r.left, r.top, r.right - r.left, r.bottom - r.top);
    Gdiplus::Size sz;
    rp.GetSize(&sz);
    return sz;
}

bool GameWindow::IsMouseInWindow() const
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(*this, &p);

    RECT r;
    GetClientRect(*this, &r);
    return PtInRect(&r, p);
}

void GameWindow::GetCreateWindow(CREATESTRUCT& cs)
{
    Window::GetCreateWindow(cs);
    cs.lpszName = TEXT("Rad Game Engine");
    cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
}

void GameWindow::GetWndClass(WNDCLASS& wc)
{
    Window::GetWndClass(wc);
    wc.hbrBackground = NULL;
}

BOOL GameWindow::OnCreate(const LPCREATESTRUCT lpCreateStruct)
{
    m_Tick = GetTickCount();
    DWORD DesiredFPS = 60;
    m_DesiredElapsed = 1 * 1000 / DesiredFPS;
    // TODO Not getting desired FPS with this method - fluctuates a lot
    SetTimer(*this, 0, m_DesiredElapsed/3, nullptr);
    return TRUE;
}

void GameWindow::OnDestroy()
{
    PostQuitMessage(0);
}

void GameWindow::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(*this, &ps);
    const Gdiplus::Size sz = GetSize();

#if 0
    const HDC hMemDC = CreateCompatibleDC(ps.hdc);
    const HBITMAP hBmp = CreateCompatibleBitmap(ps.hdc, sz.Width, sz.Height);
    const HGDIOBJ hOld = SelectObject(hMemDC, hBmp);

    {
        Gdiplus::Graphics g(hMemDC);
        OnDraw(&g);
    }

    BitBlt(ps.hdc, 0, 0, sz.Width, sz.Height, hMemDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, hOld);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);
#else
    Gdiplus::Bitmap bmp(sz.Width, sz.Height);
    {
        Gdiplus::Graphics gmem(&bmp);
        OnDraw(&gmem);
    }

    Gdiplus::Graphics g(ps.hdc);
    g.DrawImage(&bmp, 0, 0, sz.Width, sz.Height);
#endif

    EndPaint(*this, &ps);
}

void GameWindow::OnKeyDown(UINT vk, int cRepeat, UINT flags)
{
    if (!(flags & KF_REPEAT) && vk < m_KeyPressed.size())
        m_KeyPressed.set(vk);
}

void GameWindow::OnLButtonDown(int x, int y, UINT keyFlags)
{
    m_mouse.X = x;
    m_mouse.Y = y;
    m_KeyPressed.set(VK_LBUTTON);
}

void GameWindow::OnRButtonDown(int x, int y, UINT keyFlags)
{
    m_mouse.X = x;
    m_mouse.Y = y;
    m_KeyPressed.set(VK_RBUTTON);
}

void GameWindow::OnMButtonDown(int x, int y, UINT keyFlags)
{
    m_mouse.X = x;
    m_mouse.Y = y;
    m_KeyPressed.set(VK_MBUTTON);
}

void GameWindow::OnMouseMove(int x, int y, UINT keyFlags)
{
    m_mouse.X = x;
    m_mouse.Y = y;
}

void GameWindow::OnTimer(UINT ivv)
{
    const DWORD Tick = GetTickCount();
    const DWORD TickDiff = Tick - m_Tick;
    if (TickDiff >= m_DesiredElapsed/2)
    {
        m_Tick = Tick;
        m_TickHist.push_back(TickDiff);
        if (m_TickHist.size() > 20)
            m_TickHist.erase(m_TickHist.begin(), m_TickHist.begin() + m_TickHist.size() - 20);
        Update(TickDiff);
        m_KeyPressed.reset();
        InvalidateRect(*this, nullptr, TRUE);
    }
}

LRESULT GameWindow::HandleMessage(const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(WM_CREATE, OnCreate);
        HANDLE_MSG(WM_DESTROY, OnDestroy);
        HANDLE_MSG(WM_PAINT, OnPaint);
        HANDLE_MSG(WM_KEYDOWN, OnKeyDown);
        HANDLE_MSG(WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(WM_RBUTTONDOWN, OnRButtonDown);
        HANDLE_MSG(WM_MBUTTONDOWN, OnMButtonDown);
        HANDLE_MSG(WM_MOUSEMOVE, OnMouseMove);
        HANDLE_MSG(WM_TIMER, OnTimer);
        HANDLE_DEF(Window::HandleMessage);
    }
}

bool Run(_In_ const LPCTSTR lpCmdLine, _In_ const int nShowCmd)
{
    if (GameWindow::Register() == 0)
    {
        MessageBox(NULL, TEXT("Error registering window class"), TEXT("Rad Game Engine"), MB_ICONERROR | MB_OK);
        return false;
    }
    GameWindow* prw = GameWindow::DoInit(lpCmdLine);
    if (prw == nullptr)
    {
        MessageBox(NULL, TEXT("Error creating root window"), TEXT("Rad Game Engine"), MB_ICONERROR | MB_OK);
        return false;
    }

    ShowWindow(*prw, nShowCmd);
    return true;
}
