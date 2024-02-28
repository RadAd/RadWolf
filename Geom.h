#pragma once

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

inline PosF operator+(const PosF p1, const PosF p2)
{
    return PosF({ p1.x + p2.x, p1.y + p2.y });
}

inline PosF operator-(const PosF p1, const PosF p2)
{
    return PosF({ p1.x - p2.x, p1.y - p2.y });
}

inline PosF& operator+=(PosF& p1, const PosF p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

inline PosF& operator-=(PosF& p1, const PosF p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    return p1;
}

inline PosF operator*(PosF p, const double v)
{
    p.x *= v;
    p.y *= v;
    return p;
}

inline PosF operator*(const PosF p1, const PosF p2)
{
    return PosF({ p1.x * p2.x, p1.y * p2.y });
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
