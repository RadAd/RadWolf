#define NOMINMAX
#include "RGE/GameWindow.h"
#include "RGE/Format.h"
#include "RGE/GdiplusUtils.h"
#include "RGE/Geometry.h"

//#define _USE_MATH_DEFINES
//#include <math.h>
#define M_PI       3.14159265358979323846   // pi

#include <cmath>
#include <algorithm>

template <typename T>
inline T sq(T x)
{
    return x * x;
}

template <typename T>
inline int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline double deg2rad(double d)
{
    return d * M_PI / 180.0;
}

double naive_lerp(double a, double b, double t)
{
    return a + t * (b - a);
}

struct Size
{
    int w;
    int h;
};

struct Pos
{
    int x;
    int y;
};

struct PosF
{
    double x;
    double y;
};

template<class T>
class Map
{
public:
    Map()
    {
    }

    Map(Size sz)
        : m_sz(sz), m_Data(sz.w * sz.h)
    {
    }

    Size size() const { return m_sz; }

    bool valid(Pos p) const
    {
        return p.x >= 0 && p.y >= 0 && p.x < m_sz.w && p.y < m_sz.h;
    }

    T& operator[](Pos p)
    {
        return m_Data[ToIndex(p)];
    }

    const T& operator[](Pos p) const
    {
        return m_Data[ToIndex(p)];
    }

private:
    int ToIndex(Pos p) const
    {
        return p.y * m_sz.w + p.x;
    }

    Size m_sz;
    std::vector<T> m_Data;
};

Gdiplus::Point ToD(const Gdiplus::PointF p)
{
    return Gdiplus::Point(INT(p.X + 0.5), INT(p.Y + 0.5));
}

Gdiplus::Point operator*(const Pos p, const Gdiplus::Size sz)
{
    return Gdiplus::Point(p.x * sz.Width, p.y * sz.Height);
}

PosF operator+(const PosF p1, const PosF p2)
{
    return PosF({ p1.x + p2.x, p1.y + p2.y });
}

PosF operator-(const PosF p1, const PosF p2)
{
    return PosF({ p1.x - p2.x, p1.y - p2.y });
}

PosF& operator+=(PosF& p1, const PosF p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

PosF& operator-=(PosF& p1, const PosF p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    return p1;
}

PosF operator*(PosF p, const double v)
{
    p.x *= v;
    p.y *= v;
    return p;
}

Gdiplus::PointF operator*(const PosF p, const Gdiplus::Size sz)
{
    return Gdiplus::PointF(Gdiplus::REAL(p.x * sz.Width), Gdiplus::REAL(p.y * sz.Height));
}

PosF operator*(const PosF p1, const PosF p2)
{
    return PosF({ p1.x * p2.x, p1.y * p2.y });
}

Gdiplus::PointF operator*(const Gdiplus::PointF p, const Gdiplus::Size sz)
{
    return Gdiplus::PointF(p.X * sz.Width, p.Y * sz.Height);
}

Gdiplus::PointF operator*(const Gdiplus::PointF p1, const Gdiplus::PointF p2)
{
    return Gdiplus::PointF(p1.X * p2.X, p1.Y * p2.Y);
}

inline Gdiplus::PointF operator*(Gdiplus::PointF p, Gdiplus::REAL f)
{
    p.X *= f;
    p.Y *= f;
    return p;
}

inline double lensq(const PosF p)
{
    return sq(p.x) + sq(p.y);
}

inline double len(const PosF p)
{
    return sqrt(lensq(p));
}

inline PosF normalize(PosF p)
{
    const double l = len(p);
    p.x /= l;
    p.y /= l;
    return p;
}

inline PosF FromAngle(double a)
{
    return PosF({ std::cos(-a), std::sin(-a) });
}

inline Gdiplus::PointF ToGdiplus(PosF p)
{
    return Gdiplus::PointF(Gdiplus::REAL(p.x), Gdiplus::REAL(p.y));
}

Gdiplus::Point ray(const Gdiplus::Point s, double a, double l)
{
    //Gdiplus::PointF dir(Gdiplus::REAL(std::cos(-a)), Gdiplus::REAL(std::sin(-a)));
    Gdiplus::PointF dir(ToGdiplus(FromAngle(a)));
    //return Gdiplus::Point(INT(s.X + dir.X * l + 0.5), INT(s.Y + dir.Y * l + 0.5));
    return ToD(ToF(s) + dir * Gdiplus::REAL(l) + Gdiplus::PointF(0.5, 0.5));
}

struct Intersection
{
    Pos map;
    double len;
    PosF point;
};

template <class F>
Intersection intersect(const PosF vRayStart, double a, F f)
{
    const PosF vRayDir(normalize(PosF({ std::cos(a), -std::sin(a) })));
    const PosF vRayUnitStepSize({ std::sqrt(sq(vRayDir.y / vRayDir.x) + 1), std::sqrt(sq(vRayDir.x / vRayDir.y) + 1) });

    Pos vMapCheck({ int(vRayStart.x), int(vRayStart.y) });
    const Pos vStep({ sgn(vRayDir.x), sgn(vRayDir.y) });

    PosF vRayLength1D(
        PosF({
            vStep.x < 0 ? vRayStart.x - vMapCheck.x : vMapCheck.x + 1 - vRayStart.x,
            vStep.y < 0 ? vRayStart.y - vMapCheck.y : vMapCheck.y + 1 - vRayStart.y
            }) * vRayUnitStepSize);

    double d;
    do
    {
        if (vRayLength1D.x < vRayLength1D.y)
        {
            d = vRayLength1D.x;
            vMapCheck.x += vStep.x;
            vRayLength1D.x += vRayUnitStepSize.x;
        }
        else
        {
            d = vRayLength1D.y;
            vMapCheck.y += vStep.y;
            vRayLength1D.y += vRayUnitStepSize.y;
        }
    } while (!f(vMapCheck));

    return { vMapCheck, d, vRayStart + vRayDir * d };
}

class RadWolf : public GameWindow
{
public:
    void Init(const LPCTSTR lpCmdLine) override
    {
        Create(TEXT("RadWolf"), 800, 600);
        m_Map = Map<Gdiplus::Color>(Size({ 10, 10 }));
        Pos p;
        for (p.y = 0; p.y < m_Map.size().h; ++p.y)
        {
            for (p.x = 0; p.x < m_Map.size().w; ++p.x)
            {
                m_Map[p] = Gdiplus::Color::White;
            }
        }
        p.y = 2;
        for (p.x = 2; p.x < 8; ++p.x)
        {
            m_Map[p] = Gdiplus::Color::Green;
        }
        p.x = 8;
        for (p.y = 5; p.y < 8; ++p.y)
        {
            m_Map[p] = Gdiplus::Color::Red;
        }
        m_player = PosF({ 5.5, 7.5 });
    }

    void Update(DWORD ms) override
    {
        if (GetKeyPressed(VK_ESCAPE)) Close();
        if (GetKeyDown('A')) m_a += deg2rad(1) * (ms / 10.0);
        if (GetKeyDown('D')) m_a -= deg2rad(1) * (ms / 10.0);
        if (GetKeyDown('W'))
        {
            const Intersection inter = intersect(m_player, m_a, [&m = m_Map](Pos vMapCheck) { return !m.valid(vMapCheck) || m[vMapCheck].GetValue() != Gdiplus::Color::White; });
            m_player += FromAngle(m_a) * std::min((ms / 100.0), inter.len - 0.1);
        }
        if (GetKeyDown('S'))
        {
            const Intersection inter = intersect(m_player, m_a - deg2rad(180), [&m = m_Map](Pos vMapCheck) { return !m.valid(vMapCheck) || m[vMapCheck].GetValue() != Gdiplus::Color::White; });
            m_player -= FromAngle(m_a) * std::min((ms / 100.0), inter.len - 0.1);
        }
    }

    void OnDraw(Gdiplus::Graphics* const g) const override
    {
        g->Clear(Gdiplus::Color::White);

        const Gdiplus::Size sz = GetSize();
        const double aspect = double(sz.Height) / sz.Width;
        const Gdiplus::SolidBrush FontBrush(Gdiplus::Color::Black);
        const Gdiplus::Font Font(L"Arial", 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

        const Gdiplus::StringFormat RightJustify = SetAlignment(nullptr, Gdiplus::StringAlignmentFar).get();
        g->DrawString(Format(L"FPS %.2f", GetFPS()).c_str(), -1, &Font, { static_cast<Gdiplus::REAL>(sz.Width), 0 }, &RightJustify, &FontBrush);

        std::vector<Intersection> intersections(sz.Width);
        const double fov = 70;
        for (int x = 0; x < sz.Width; ++x)
        {
            const double i = naive_lerp(fov / 2, -fov / 2, double(x) / sz.Width);
            const Intersection inter = intersect(m_player, m_a + deg2rad(i), [&m = m_Map](Pos vMapCheck) { return !m.valid(vMapCheck) || m[vMapCheck].GetValue() != Gdiplus::Color::White; });
            intersections[x] = inter;
        }

        {
            for (int x = 0; x < sz.Width; ++x)
            {
                const Intersection& inter = intersections[x];
                const Gdiplus::Color c = m_Map.valid(inter.map) ? m_Map[inter.map] : Gdiplus::Color::Gray;
                const Gdiplus::Pen pen_wall(c);

                //const double d = len(pt2 - m_player); // TODO wrong distance calculation - this produces a fish-eye lens
                const double i = naive_lerp(-fov / 2, fov / 2, double(x) / sz.Width);
                const double d = len(inter.point - m_player) * std::cos(deg2rad(i));
                const Gdiplus::PointF h(0, Gdiplus::REAL(sz.Height * aspect / d));

                const Gdiplus::PointF m(Gdiplus::REAL(x), Gdiplus::REAL(sz.Height) / 2);
                g->DrawLine(&pen_wall, m + h, m - h);
            }
        }

        const Gdiplus::Size tsz(10, 10);
        {
            const Gdiplus::Pen pen_grid(Gdiplus::Color::Black);

            Pos p;
            for (p.y = 0; p.y < m_Map.size().h; ++p.y)
            {
                for (p.x = 0; p.x < m_Map.size().w; ++p.x)
                {
                    const Gdiplus::SolidBrush brush_grid(m_Map[p]);
                    const Gdiplus::Rect r(p * tsz, tsz);
                    g->FillRectangle(&brush_grid, r);
                    g->DrawRectangle(&pen_grid, r);
                }
            }
        }

        {
            const Gdiplus::Pen pen_intersection(Gdiplus::Color::Blue);
            const Gdiplus::Point pt1(ToD(m_player * tsz));
            for (int x = 0; x < sz.Width; ++x)
            {
                const PosF pt2 = intersections[x].point;
                g->DrawLine(&pen_intersection, pt1, ToD(pt2 * tsz));
            }

            const Gdiplus::Pen pen_ray(Gdiplus::Color::Red);
            g->DrawLine(&pen_ray, pt1, ray(pt1, m_a, tsz.Width * 3));
            const Gdiplus::SolidBrush brush_player(Gdiplus::Color::Red);
            g->FillEllipse(&brush_player, pt1.X - 5, pt1.Y - 5, 10, 10);
        }
    }

private:
    Map<Gdiplus::Color> m_Map;
    PosF m_player;
    double m_a = deg2rad(80);
};

RadWolf game;

#include <tchar.h>
int WINAPI RGEWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd);
int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
    return RGEWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
