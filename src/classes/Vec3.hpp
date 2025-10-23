// Based on code from https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef VEC3_H
#define VEC3_H

#include "Common.hpp"
#include <cmath>
#include <iostream>

// Operator definitions
class Vec3;
inline Vec3 operator+(const Vec3 &a, const Vec3 &b);
inline Vec3 operator-(const Vec3 &a, const Vec3 &b);
inline Vec3 operator*(const Vec3 &a, const Vec3 &b);
inline Vec3 operator*(const num s, const Vec3 &v);
inline Vec3 operator*(const Vec3 &v, const num s);
inline Vec3 operator/(const Vec3 &v, const num s);

class Vec3 {
  public:
    num x = 0;
    num y = 0;
    num z = 0;

    // Zero constructor
    explicit constexpr Vec3() = default;
    explicit constexpr Vec3(const num val) : x(val), y(val), z(val) {}
    explicit constexpr Vec3(const num x, const num y, const num z) : x(x), y(y), z(z) {}

    inline Vec3 operator-() const { 
        return Vec3(-x, -y, -z); 
    }

    inline Vec3& operator+=(const Vec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    inline Vec3& operator*=(num t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    inline Vec3& operator/=(num t) {
        return (*this *= (static_cast<num>(1) / t));
    }

    inline num length() const {
        return std::sqrt(length_squared());
    }

    inline num length_squared() const {
        return (x * x) + (y * y) + (z * z);
    }

    inline Vec3 into_unit() const {
        return *this / length();
    }
    
    inline num dot(const Vec3 &rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
    }

    inline Vec3 cross(const Vec3 &rhs) const {
        return Vec3(
            (y * rhs.z) - (z * rhs.y),
            (z * rhs.x) - (x * rhs.z),
            (x * rhs.y) - (y * rhs.x)
        );
    }

    // ! Double check this does what it should.
    inline Vec3 reflect(const Vec3 &n) const {
        const Vec3 v(*this);
        return v - (2 * v.dot(n) * n);
    }

    // ? Should this function name be shortened?
    inline Vec3 into_reciprocal() const {
        return Vec3(
            1 / x,
            1 / y,
            1 / z
        );
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << 'Vec3(' << v.x << ', ' << v.y << ', ' << v.z << ')';
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(num t, const Vec3& v) {
    return Vec3(t*v.x, t*v.y, t*v.z);
}

inline Vec3 operator*(const Vec3& v, num t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, num t) {
    return (static_cast<num>(1) / t) * v;
}

#endif