#ifndef COLOUR_H
#define COLOUR_H

#include "../common.hpp"
#include "vec3.hpp"

#include <cstdint>

namespace texture {
    class Colour {
    public:
        num r = 0.0;    // doubles for accuracy during calculations, can round later?
        num g = 0.0;
        num b = 0.0;
        num a = 1.0; // Alpha channel, range: [0, 1]

        explicit constexpr Colour() noexcept = default;

        explicit constexpr Colour(const num r, const num g, const num b, const num a = 1) noexcept
        : r(r)
        , g(g)
        , b(b)
        , a(a) {}

        Colour& operator+=(const Colour& rhs) {
            // TODO: Implement RGBA addition logic.
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }

        // ! Remove this function if unneeded.
        // Vec3 rgb() {
        // 	return Vec3(r, g, b);
        // }

        static constexpr Colour black() noexcept {
            return Colour(0, 0, 0);
        }

        static constexpr Colour white() noexcept {
            return Colour(1.0, 1.0, 1.0);
        }

        static constexpr Colour red() noexcept {
            return Colour(1.0, 0, 0);
        }

        static constexpr Colour green() noexcept {
            return Colour(0, 1.0, 0);
        }

        static constexpr Colour blue() noexcept {
            return Colour(0, 0, 1.0);
        }

        static constexpr Colour yellow() noexcept {
            return Colour(1.0, 1.0, 0);
        }

        static constexpr Colour cyan() noexcept {
            return Colour(0, 1.0, 1.0);
        }

        static constexpr Colour magenta() noexcept {
            return Colour(1.0, 0, 1.0);
        }
    };

    // vector addition
    inline Colour operator+(const Colour& u, const Colour& v) noexcept {
        return Colour(u.r + v.r, u.g + v.g, u.b + v.b);
    }

    // scalar multiplication
    inline Colour operator*(num t, const Colour & v) noexcept {
        return Colour(t * v.r, t * v.g, t * v.b);
    }

    inline Colour operator*(const Colour & v, num t) noexcept {
        return t * v;
    }
} // namespace texture

#endif // COLOUR_H