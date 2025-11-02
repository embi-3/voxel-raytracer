// Based on code from https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef VEC3_H
#define VEC3_H

#include "../common.hpp"
#include <cmath>
#include <iostream>

namespace geometry {
    class Vec3 {
    public:
        // Zero constructor
        constexpr Vec3() noexcept = default;

        // is this necessary?
        explicit constexpr Vec3(const num val) noexcept
        : x(val)
        , y(val)
        , z(val) {}

        constexpr Vec3(const num x, const num y, const num z) noexcept
        : x(x)
        , y(y)
        , z(z) {}

        // Inverse operator
        Vec3 operator-() const noexcept {
            return Vec3(-x, -y, -z);
        }

        // Vector addition assignment operator
        Vec3& operator+=(const Vec3& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        // Vector subtraction assignment operator
        Vec3& operator-=(const Vec3& rhs) noexcept {
            return *this += -rhs;
        }

        // Scalar multiplication assignment operator
        Vec3& operator*=(num t) noexcept {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        // Scalar division assignment operator
        Vec3& operator/=(num t) {
            return (*this *= (static_cast<num>(1) / t));
        }

        // Magnitude
        num length() const noexcept {
            return std::sqrt(length_squared());
        }

        // Magnitude squared
        num length_squared() const noexcept {
            return (x * x) + (y * y) + (z * z);
        }

        // Dot product
        num dot(const Vec3& rhs) const noexcept {
            return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
        }

        // Cross product
        Vec3 cross(const Vec3& rhs) const noexcept {
            return Vec3((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
        }

        // ? Should this function name be shortened?
        Vec3 into_reciprocal() const {
            return Vec3(1 / x, 1 / y, 1 / z);
        }

        // ! Double check this does what it should.
        Vec3 reflect(const Vec3& n) const;

        Vec3 normalise() const;

        // Components
        num x = 0;
        num y = 0;
        num z = 0;
    };

    using Point3 = Vec3;

    // Vector Non-Member Utility Functions
    inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
        return out << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    inline Vec3 operator+(const Vec3& u, const Vec3& v) noexcept {
        return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    inline Vec3 operator-(const Vec3& u, const Vec3& v) noexcept {
        return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    inline Vec3 operator*(const Vec3& u, const Vec3& v) noexcept {
        return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
    }

    inline Vec3 operator*(num t, const Vec3& v) noexcept {
        return Vec3(t * v.x, t * v.y, t * v.z);
    }

    inline Vec3 operator*(const Vec3& v, num t) noexcept {
        return t * v;
    }

    inline Vec3 operator/(const Vec3& v, num t) {
        return (static_cast<num>(1) / t) * v;
    }
} // namespace geometry

#endif