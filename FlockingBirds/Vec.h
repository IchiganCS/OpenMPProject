#ifndef VEC_H
#define VEC_H

class Vec
{
  public:
    float x = 0, y = 0;

    float distanceTo(const Vec& other);
    void toLength(float length);
    void normalize();
    Vec normalized();
    float length();
    void limitLength(float length);
    void scale(float scale);

    /// Calculates the angle of the vector relative to (1, 0) measured counter-clockwise (e.g. (0, 1) is Pi/2)
    float angle();
    bool operator==(const Vec& other) const = default;
    Vec operator-(const Vec& other) const;
    Vec operator+(const Vec& other) const;
    Vec operator*(float other) const;
    Vec operator/(float other) const;
    Vec& operator*=(float other);
    Vec& operator+=(const Vec& other);
};

#endif