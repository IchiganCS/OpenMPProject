#ifndef VEC_H
#define VEC_H

#include <math.h>

class Vec {
public:
  float X, Y;
  Vec() {
    X = 0;
    Y = 0;
  }

  inline float length() { return sqrtf(X * X + Y * Y); }

  inline Vec getNormalisedVec() {
    Vec v;
    v.X = X / length();
    v.Y = Y / length();
    return v;
  }

  static float distance(Vec &a, Vec &b) {
    float dist =
        sqrtf(((a.X - b.X) * (a.X - b.X)) + ((a.Y - b.Y) * (a.Y - b.Y)));
  }
};

#endif