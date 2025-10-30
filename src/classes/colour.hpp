#ifndef COLOUR_H
#define COLOUR_H

#include "../common.hpp"
#include "vec3.hpp"

#include <cstdint>

namespace texture {
	class Colour {
		public:
			num r = 0;
			num g = 0;
			num b = 0;
			num a = 0; // Alpha channel, range: [0, 1]

			explicit constexpr Colour() noexcept = default;
			explicit constexpr Colour(const num r, const num g, const num b, const num a = 1) noexcept : r(r), g(g), b(b), a(a) {}

			Colour& operator+=([[maybe_unused]] const Colour& rhs) {
				// TODO: Implement RGBA addition logic.
				return *this;
			}

            Colour& operator*=(num s) noexcept {
                r *= s;
                g *= s;
                b *= s;
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
				return Colour(255, 255, 255);
			}

			static constexpr Colour red() noexcept {
				return Colour(255, 0, 0);
			}

			static constexpr Colour green() noexcept {
				return Colour(0, 255, 0);
			}

			static constexpr Colour blue() noexcept {
				return Colour(0, 0, 255);
			}

			static constexpr Colour yellow() noexcept {
				return Colour(255, 255, 0);
			}

			static constexpr Colour cyan() noexcept {
				return Colour(0, 255, 255);
			}

			static constexpr Colour magenta() noexcept {
				return Colour(255, 0, 255);
			}
	};

    // ! This currently takes the average of the alpha channel for both colours.
    // ! How this implementation should handle addition should be discussed later.
    inline Colour operator+(const Colour& u, const Colour& v) noexcept {
        return Colour(u.r + v.r, u.g + v.g, u.b + v.b, (u.a + v.a) / 2);
    }

    inline Colour operator*(num s, const Colour& c) noexcept {
        return Colour(s*c.r, s*c.g, s*c.b, c.a);
    }
}

#endif // COLOUR_H