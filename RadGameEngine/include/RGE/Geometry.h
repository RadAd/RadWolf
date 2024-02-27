#pragma once

template <class T>
inline void Clamp(T& v, T min, T max)
{
    if (v < min)
        v = min;
    else if (v > max)
        v = max;
}

void Clamp(Gdiplus::Point& p, Gdiplus::Rect r)
{
    Clamp(p.X, r.X, r.X + r.Width);
    Clamp(p.Y, r.Y, r.Y + r.Height);
}

int Sign(bool neg, bool pos)
{
    if (pos)
        return 1;
    else if (neg)
        return -1;
    else
        return 0;
}

inline Gdiplus::Point Rotate(const Gdiplus::Point p, double angle)
{
    const double s = sin(angle);
    const double c = cos(angle);

    const double xnew = p.X * c - p.Y * s;
    const double ynew = p.X * s + p.Y * c;

    return { lround(xnew), lround(ynew) };
}
