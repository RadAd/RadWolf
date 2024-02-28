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
inline Pos<T> operator*(const Pos<T> p, const T v)
{
    return Pos<T>({ p.x * v, p.y * v });
}

template <class T>
inline Pos<T> operator/(const Pos<T> p, const T v)
{
    return Pos<T>({ p.x / v, p.y / v });
}

template <class T>
inline Pos<T> operator*(const Pos<T> p1, const Pos<T> p2)
{
    return Pos<T>({ p1.x * p2.x, p1.y * p2.y });
}

inline double lensq(const Pos<double> p)
{
    return sq(p.x) + sq(p.y);
}

inline double len(const Pos<double> p)
{
    return sqrt(lensq(p));
}

inline Pos<double> normalize(const Pos<double> p)
{
    const double l = len(p);
    return p / l;
}

inline Pos<double> FromAngle(double a)
{
    return Pos<double>({ std::cos(-a), std::sin(-a) });
}
