// Based on code from https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef VEC3_H
#define VEC3_H

#include "../common.hpp"
#include <cmath>
#include <iostream>

namespace geometry {
    template<typename T>
    class Triple {
    public:
        // Zero constructor
        constexpr Triple() noexcept = default;

        constexpr Triple(const T v) noexcept
        : x(v)
        , y(v)
        , z(v) {}

        constexpr Triple(const T x, const T y, const T z) noexcept
        : x(x)
        , y(y)
        , z(z) {}

        // Inverse operator
        Triple operator-() const noexcept {
            return Triple(-x, -y, -z);
        }

        // Vector addition assignment operator
        Triple& operator+=(const Triple& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        // Vector subtraction assignment operator
        Triple& operator-=(const Triple& rhs) noexcept {
            return *this += -rhs;
        }

        // Scalar multiplication assignment operator
        Triple& operator*=(T t) noexcept {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        // Scalar division assignment operator
        Triple& operator/=(T t) {
            return (*this *= (static_cast<T>(1) / t));
        }

        // Magnitude
        T length() const noexcept {
            return std::sqrt(length_squared());
        }

        // Magnitude squared
        T length_squared() const noexcept {
            return (x * x) + (y * y) + (z * z);
        }

        // Dot product
        T dot(const Triple& rhs) const noexcept {
            return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
        }

        // Cross product
        Triple cross(const Triple& rhs) const noexcept {
            return Triple((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
        }

        // ? Should this function name be shortened?
        Triple into_reciprocal() const {
            return Triple(1 / x, 1 / y, 1 / z);
        }

        // ! Double check this does what it should.

        // ! Double check this does what it should.
        Triple reflect(const Triple& n) const {
            const Triple v(*this);
            return v - (2 * v.dot(n) * n);
        }

        Triple normalise() const {
            return *this / length();
        }

        // Components
        T x = 0;
        T y = 0;
        T z = 0;
    };

    using Vec3 = Triple<num>;
    using Int3 = Triple<int>;
    using Coordinate = Triple<unsigned int>; // ! May have to make this a long if larger models are needed.

    // Vector Non-Member Utility Functions
    template<typename T>
    inline std::ostream& operator<<(std::ostream& out, const Triple<T>& v) {
        return out << "Triple(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    template<typename T>
    inline Triple<T> operator+(const Triple<T>& u, const Triple<T>& v) noexcept {
        return Triple(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    template<typename T>
    inline Triple<T> operator-(const Triple<T>& u, const Triple<T>& v) noexcept {
        return Triple(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    template<typename T>
    inline Triple<T> operator*(const Triple<T>& u, const Triple<T>& v) noexcept {
        return Triple(u.x * v.x, u.y * v.y, u.z * v.z);
    }

    template<typename T>
    inline Triple<T> operator*(num t, const Triple<T>& v) noexcept {
        return Triple(t * v.x, t * v.y, t * v.z);
    }

    template<typename T>
    inline Triple<T> operator*(const Triple<T>& v, num t) noexcept {
        return static_cast<T>(t) * v;
    }

    template<typename T>
    inline Triple<T> operator/(const Triple<T>& v, num t) {
        return (static_cast<T>(1) / t) * v;
    }
} // namespace geometry

#endif