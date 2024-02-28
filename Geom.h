#pragma once

struct Size
{
    int w;
    int h;
};

template <class T>
struct Pos
{
    T x;
    T y;
};

template <class T>
inline Pos<T> operator+(const Pos<T> p1, const Pos<T> p2)
{
    return Pos<T>({ p1.x + p2.x, p1.y + p2.y });
}

template <class T>
inline Pos<T> operator-(const Pos<T> p1, const Pos<T> p2)
{
    return Pos<T>({ p1.x - p2.x, p1.y - p2.y });
}

template <class T>
inline Pos<T>& operator+=(Pos<T>& p1, const Pos<T> p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

template <class T>
inline Pos<T>& operator-=(Pos<T>& p1, const Pos<T> p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    return p1;
}

template <class T>
inline Pos<T> operator*(Pos<T> p, const double v)
{
    p.x *= v;
    p.y *= v;
    return p;
}

template <class T>
inline Pos<T> operator*(const Pos<T> p1, const Pos<T> p2)
{
    return Pos<T>({ p1.x * p2.x, p1.y * p2.y });
}

template <class T>
inline double lensq(const Pos<T> p)
{
    return sq(p.x) + sq(p.y);
}

template <class T>
inline double len(const Pos<T> p)
{
    return sqrt(lensq(p));
}

template <class T>
inline Pos<T> normalize(Pos<T> p)
{
    const double l = len(p);
    p.x /= l;
    p.y /= l;
    return p;
}

template <class T>
inline Pos<T> FromAngle(double a)
{
    return Pos<T>({ std::cos(-a), std::sin(-a) });
}
