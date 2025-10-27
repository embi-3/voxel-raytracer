#ifndef COLOUR_H
#define COLOUR_H

#include "Common.hpp"
#include "Vec3.hpp"

class Colour {
	public:
		// ? Should we make these int8_t for compactness?
		int r = 0;
		int g = 0;
		int b = 0;
		num a = 0; // Alpha channel, range: [0, 1]

		explicit constexpr Colour() = default;
        explicit constexpr Colour(const num r, const num g, const num b) : r(r), g(g), b(b), a(1) {}
        explicit constexpr Colour(const num r, const num g, const num b, const num a) : r(r), g(g), b(b), a(a) {}
		
		Colour& operator+=(const Colour& rhs) {
			// TODO: Implement RGBA addition logic.
			return *this;
		}
		
		// ! Remove this function if unneeded.
		// Vec3 rgb() {
		// 	return Vec3(r, g, b);
		// }

		static Colour black() {
			return Colour(0, 0, 0);
		}

		static Colour white() {
			return Colour(255, 255, 255);
		}

		static Colour red() {
			return Colour(255, 0, 0);
		}

		static Colour green() {
			return Colour(0, 255, 0);
		}

		static Colour blue() {
			return Colour(0, 0, 255);
		}

		static Colour yellow() {
			return Colour(255, 255, 0);
		}

		static Colour cyan() {
			return Colour(0, 255, 255);
		}

		static Colour magenta() {
			return Colour(255, 0, 255);
		}
};

#endif // COLOUR_H