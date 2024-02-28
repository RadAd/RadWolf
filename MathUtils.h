#pragma once

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

inline double naive_lerp(double a, double b, double t)
{
    return a + t * (b - a);
}
