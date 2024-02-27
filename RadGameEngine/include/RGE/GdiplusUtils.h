#pragma once
#include <memory>

inline std::unique_ptr<Gdiplus::StringFormat> SetAlignment(const Gdiplus::StringFormat* osf, Gdiplus::StringAlignment align)
{
    if (osf == nullptr)
        osf = Gdiplus::StringFormat::GenericDefault();
    std::unique_ptr<Gdiplus::StringFormat> sf(osf->Clone());
    sf->SetAlignment(align);
    return sf;
}

inline std::unique_ptr<Gdiplus::StringFormat> SetLineAlignment(const Gdiplus::StringFormat* osf, Gdiplus::StringAlignment align)
{
    if (osf == nullptr)
        osf = Gdiplus::StringFormat::GenericDefault();
    std::unique_ptr<Gdiplus::StringFormat> sf(osf->Clone());
    sf->SetLineAlignment(align);
    return sf;
}

inline Gdiplus::SizeF ToF(Gdiplus::Size s)
{
    return { static_cast<Gdiplus::REAL>(s.Width), static_cast<Gdiplus::REAL>(s.Height) };
}

inline Gdiplus::PointF ToF(Gdiplus::Point p)
{
    return { static_cast<Gdiplus::REAL>(p.X), static_cast<Gdiplus::REAL>(p.Y) };
}

inline Gdiplus::Point& operator+=(Gdiplus::Point& p1, Gdiplus::Point p2)
{
    p1 = p1 + p2;
    return p1;
}

inline Gdiplus::Point& operator-=(Gdiplus::Point& p1, Gdiplus::Point p2)
{
    p1 = p1 - p2;
    return p1;
}

inline Gdiplus::Point operator*(Gdiplus::Point p, INT f)
{
    p.X *= f;
    p.Y *= f;
    return p;
}

inline Gdiplus::Point operator/(Gdiplus::Point p, INT f)
{
    p.X /= f;
    p.Y /= f;
    return p;
}

inline Gdiplus::PointF& operator+=(Gdiplus::PointF& p1, Gdiplus::PointF p2)
{
    p1 = p1 + p2;
    return p1;
}

inline Gdiplus::PointF& operator-=(Gdiplus::PointF& p1, Gdiplus::PointF p2)
{
    p1 = p1 - p2;
    return p1;
}

inline Gdiplus::PointF operator*(Gdiplus::PointF p, INT f)
{
    p.X *= f;
    p.Y *= f;
    return p;
}

inline Gdiplus::PointF operator/(Gdiplus::PointF p, INT f)
{
    p.X /= f;
    p.Y /= f;
    return p;
}

inline Gdiplus::Size& operator+=(Gdiplus::Size& s1, Gdiplus::Size s2)
{
    s1 = s1 + s2;
    return s1;
}

inline Gdiplus::Size& operator-=(Gdiplus::Size& s1, Gdiplus::Size s2)
{
    s1 = s1 - s2;
    return s1;
}

inline Gdiplus::Size operator*(Gdiplus::Size sz, INT f)
{
    sz.Width *= f;
    sz.Height *= f;
    return sz;
}

inline Gdiplus::Size operator/(Gdiplus::Size sz, INT f)
{
    sz.Width /= f;
    sz.Height /= f;
    return sz;
}

inline Gdiplus::SizeF& operator+=(Gdiplus::SizeF& s1, Gdiplus::SizeF s2)
{
    s1 = s1 + s2;
    return s1;
}

inline Gdiplus::SizeF& operator-=(Gdiplus::SizeF& s1, Gdiplus::SizeF s2)
{
    s1 = s1 - s2;
    return s1;
}

inline Gdiplus::SizeF operator*(Gdiplus::SizeF sz, INT f)
{
    sz.Width *= f;
    sz.Height *= f;
    return sz;
}

inline Gdiplus::SizeF operator/(Gdiplus::SizeF sz, INT f)
{
    sz.Width /= f;
    sz.Height /= f;
    return sz;
}

class StringLines
{
public:
    StringLines(Gdiplus::Graphics* const g, Gdiplus::PointF start, const Gdiplus::Font* font, const Gdiplus::Brush* brush)
        : g(g), out(start), font(font), stringFormat(nullptr), brush(brush)
    {
    }

    StringLines(Gdiplus::Graphics* const g, Gdiplus::PointF start, const Gdiplus::Font* font, const Gdiplus::StringFormat* stringFormat, const Gdiplus::Brush* brush)
        : g(g), out(start), font(font), stringFormat(stringFormat), brush(brush)
    {
    }

    Gdiplus::Status DrawString(const WCHAR* string, INT length = -1)
    {
        Gdiplus::Status s = g->DrawString(string, length, font, out, stringFormat, brush);
        out.Y += font->GetHeight(g);
        return s;
    }

    Gdiplus::Status DrawString(const std::wstring& s)
    {
        return DrawString(s.c_str(), static_cast<INT>(s.length()));
    }

private:
    Gdiplus::Graphics* const g;
    Gdiplus::PointF out;
    const Gdiplus::Font* font;
    const Gdiplus::StringFormat* stringFormat;
    const Gdiplus::Brush* brush;
};
