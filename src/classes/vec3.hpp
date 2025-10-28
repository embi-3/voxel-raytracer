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
        explicit constexpr Vec3() noexcept = default;
        explicit constexpr Vec3(const num val) noexcept : x(val), y(val), z(val) {}
        explicit constexpr Vec3(const num x, const num y, const num z) noexcept : x(x), y(y), z(z) {}

        Vec3 operator-() const noexcept { 
            return Vec3(-x, -y, -z); 
        }

        Vec3& operator+=(const Vec3& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& rhs) noexcept {
            return *this += -rhs;
        }

        Vec3& operator*=(num t) noexcept {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        Vec3& operator/=(num t) {
            return (*this *= (static_cast<num>(1) / t));
        }

        num length() const noexcept {
            return std::sqrt(length_squared());
        }

        num length_squared() const noexcept {
            return (x * x) + (y * y) + (z * z);
        }

<<<<<<< HEAD
        Vec3 into_unit() const;
=======
        Vec3 into_unit() const noexcept;    // defined outside class (at the bottom)
>>>>>>> 04c2b85 (Simple ray caster (#2))

        num dot(const Vec3& rhs) const noexcept {
            return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
        }

        Vec3 cross(const Vec3& rhs) const noexcept {
            return Vec3((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
        }

        // ! Double check this does what it should.
        Vec3 reflect(const Vec3& n) const;

        // ? Should this function name be shortened?
        Vec3 into_reciprocal() const {
            return Vec3(1 / x, 1 / y, 1 / z);
        }

        // Components
        num x = 0;
        num y = 0;
        num z = 0;
    };

    // point3 is just an alias for vec3, but useful for geometric clarity in the code.
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

    inline Vec3 operator*(num t, const Vec3& v) noexcept {
        return Vec3(t * v.x, t * v.y, t * v.z);
    }

    inline Vec3 operator*(const Vec3& v, num t) noexcept {
        return t * v;
    }

    inline Vec3 operator/(const Vec3& v, num t) {
        return (static_cast<num>(1) / t) * v;
    }
<<<<<<< HEAD
=======

    inline Vec3 Vec3::into_unit() const noexcept {
        return *this / length();
    }
>>>>>>> 04c2b85 (Simple ray caster (#2))
} // namespace geometry

#endif