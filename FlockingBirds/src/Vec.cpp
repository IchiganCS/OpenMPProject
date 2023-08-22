#include "Vec.h"

#include <cmath>

float Vec::distanceTo(const Vec &other) {
  return std::sqrt((x - other.x) * (x - other.x) +
                   (y - other.y) * (y - other.y));
}

void Vec::scale(float scale) {
  x *= scale;
  y *= scale;
}

float Vec::angle() { return std::atan2(y, x); }

Vec Vec::operator-(const Vec &other) const {
  Vec res;
  res.x = x - other.x;
  res.y = y - other.y;
  return res;
}

bool Vec::operator==(const Vec &other) const {
  return x == other.x && y == other.y;
}

Vec Vec::operator+(const Vec &other) const {
  Vec res;
  res.x = x + other.x;
  res.y = y + other.y;
  return res;
}

Vec &Vec::operator+=(const Vec &other) {
  x += other.x;
  y += other.y;
  return *this;
}

void Vec::toLength(float length) {
  if (x == 0 && y == 0)
    return;

  float current = this->length();
  x *= length / current;
  y *= length / current;
}

void Vec::limitLength(float desired) {
  if (x == 0 && y == 0)
    return;

  float current = this->length();
  if (current > desired) {
    x *= desired / current;
    y *= desired / current;
  }
}

float Vec::length() { return std::sqrt(x * x + y * y); }