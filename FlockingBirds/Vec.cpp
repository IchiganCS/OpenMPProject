#include "Vec.h"

#include <cmath>
#include <numbers>

float Vec::distanceTo(const Vec& other)
{
    return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

void Vec::scale(float scale)
{
    x *= scale;
    y *= scale;
}

float Vec::angle()
{
    return std::atan2(y, x);
}

void Vec::normalize()
{
    toLength(1);
}
Vec Vec::normalized()
{
    Vec ret = {x, y};
    ret.toLength(1);
    return ret;
}

Vec Vec::operator-(const Vec& other) const
{
    Vec res;
    res.x = x - other.x;
    res.y = y - other.y;
    return res;
}
Vec Vec::operator+(const Vec& other) const
{
    Vec res;
    res.x = x + other.x;
    res.y = y + other.y;
    return res;
}

Vec& Vec::operator+=(const Vec& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vec& Vec::operator*=(float mult) {
    x *= mult;
    y *= mult;
    return *this;
}

void Vec::toLength(float length)
{
    if (x == 0 && y == 0)
        return;

    float current = this->length();
    x *= length / current;
    y *= length / current;
}

Vec Vec::operator*(float other) const
{
    Vec ret;
    ret.x = this->x * other;
    ret.y = this->y * other;
    return ret;
}
Vec Vec::operator/(float other) const
{
    Vec ret;
    ret.x = this->x / other;
    ret.y = this->y / other;
    return ret;
}

void Vec::limitLength(float desired)
{
    if (x == 0 && y == 0)
        return;

    float current = this->length();
    if (current > desired)
    {
        x *= desired / current;
        y *= desired / current;
    }
}

float Vec::length()
{
    return std::sqrt(x * x + y * y);
}
