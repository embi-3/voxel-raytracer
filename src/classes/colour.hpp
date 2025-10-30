#ifndef COLOUR_H
#define COLOUR_H

#include "../common.hpp"
#include "vec3.hpp"

#include <cstdint>

namespace texture {
	class Colour {
		public:
			uint8_t r = 0;
			uint8_t g = 0;
			uint8_t b = 0;
			num a = 0; // Alpha channel, range: [0, 1]

			explicit constexpr Colour() noexcept = default;
			explicit constexpr Colour(const uint8_t r, const uint8_t g, const uint8_t b, const num a = 1) noexcept : r(r), g(g), b(b), a(a) {}

			Colour& operator+=([[maybe_unused]] const Colour& rhs) {
				// TODO: Implement RGBA addition logic.
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

	// TODO: Implement binary operands for scalar multiplication of Colours
}

#endif // COLOUR_H