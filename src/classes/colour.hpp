#ifndef COLOUR_H
#define COLOUR_H

#include "../common.hpp"
#include "vec3.hpp"

#include <cstdint>
#include <limits>
#include <algorithm>
// format library requires clang-17
// #include <format>

// Biggest number less than 256
static const num RGB_MAX = std::nexttoward(256, 0);
static const num RGB_MIN = 0;

namespace texture {
	class Colour {
		public:
			num r = 0; // range: [0, 256)
			num g = 0; // range: [0, 256)
			num b = 0; // range: [0, 256)
			num a = 0; // range: [0, 1]

			explicit constexpr Colour() noexcept = default;
			explicit constexpr Colour(const num r, const num g, const num b, const num a = 1) noexcept : r(r), g(g), b(b), a(a) {}

			Colour& operator+=(const Colour& rhs) {
				r += rhs.r;
				g += rhs.g;
				b += rhs.b;
				a += rhs.a;
				return *this;
			}

            Colour& operator*=(num s) noexcept {
                r *= s;
                g *= s;
                b *= s;
                return *this;
            }

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

			inline Colour set_alpha(num alpha) {
				a = alpha;
				return *this;
			}

			// ! std::format not supported by compiler.
			// inline std::string to_rgb_string() {
			// 	return std::format("%d %d %d %d", rint(), gint(), bint());
			// }

			// inline std::string to_rgba_string() {
			// 	return std::format("%d %d %d %.2f", rint(), gint(), bint(), a);
			// }

			inline std::string to_rgb_string() const {
				return std::to_string(r_int()) + " " + std::to_string(g_int()) + " " + std::to_string(b_int());
			}

			// ! This does not restrict the precision of the float when printing.
			inline std::string to_rgba_string() const {
				return std::to_string(r_int()) + " " + std::to_string(g_int()) + " " + std::to_string(b_int()) + " " + std::to_string(a);
			}

			// inline bool is_valid() const {
			// 	return (r >= 0 && r < 256)
			// 		&& (g >= 0 && g < 256)
			// 		&& (b >= 0 && b < 256)
			// 		&& (a >= 0.0 && a <= 1.0);
			// }

			// static_cast always truncates the floating part of the number, rounding down.

			inline uint8_t r_int() const {
				return static_cast<uint8_t>(r);
			}

			inline uint8_t g_int() const {
				return static_cast<uint8_t>(g);
			}

			inline uint8_t b_int() const {
				return static_cast<uint8_t>(b);
			}
	};

	// Operator Overloads
	// ! The following operations always return a valid RGBA value. If you are getting unexpected
	// ! results, check for possible overflows in your arithmetic. You should avoid calling these
	// ! operators directly and use blend modes instead.

	// ? Does clamping on every operation have a significant performance penalty? 
	// ? This could be something to test.

	inline num bounded_rgb(num value) {
		return std::clamp(value, RGB_MIN, RGB_MAX);
	}

	inline num bounded_alpha(num alpha) {
		return std::clamp(alpha, 0.0, 1.0);
	}

    inline Colour operator+(const Colour& u, const Colour& v) noexcept {
		return Colour(
			bounded_rgb(u.r + v.r),
			bounded_rgb(u.g + v.g),
			bounded_rgb(u.b + v.b), 
			bounded_alpha(u.a + v.a)
		);
    }

    inline Colour operator*(const num s, const Colour& c) noexcept {
        return Colour(
			bounded_rgb(s * c.r),
			bounded_rgb(s * c.g),
			bounded_rgb(s * c.b), 
			c.a
		);
    }

	inline Colour operator*(const Colour& c, const num s) noexcept {
        return Colour(
			bounded_rgb(s * c.r),
			bounded_rgb(s * c.g),
			bounded_rgb(s * c.b), 
			c.a
		);
    }

	// Unbounded Operator Overloads
	// ! These have been left here for future testing purposes.

	// // ! These operations may produce invalid RGBA values! Be careful of overflows!
    // inline Colour operator+(const Colour& u, const Colour& v) noexcept {
    //     Colour res = Colour(u.r + v.r, u.g + v.g, u.b + v.b, max((u.a) + (u.a * v.a));
	// 	if (!res.is_valid()) {
	// 		std::cerr << "Invalid RGBA value! Received " << res.to_rgb_string();
	// 		return Colour::black();
	// 	}

	// 	return res;
    // }

	// // ! These operations may produce invalid RGBA values! Be careful of overflows!
    // inline Colour operator*(const num s, const Colour& c) noexcept {
    //     Colour res = Colour(s*c.r, s*c.g, s*c.b, c.a);
	// 	if (!res.is_valid()) {
	// 		std::cerr << "Invalid RGBA value! Received " << res.to_rgba_string();
	// 		return Colour::black();
	// 	}

	// 	return res;
    // }

	// // ! These operations may produce invalid RGBA values! Be careful of overflows!
	// inline Colour operator*(const Colour& c, const num s) noexcept {
    //     Colour res = Colour(s*c.r, s*c.g, s*c.b, c.a);
	// 	if (!res.is_valid()) {
	// 		std::cerr << "Invalid RGBA value! Received " << res.to_rgba_string();
	// 		return Colour::black();
	// 	}

	// 	return res;
    // }

	// Blend Modes
	Colour normal_blend(const Colour& u, const Colour& v) noexcept;

	Colour interpolate(const Colour& u, const Colour& v, num ratio) noexcept;
	
}

#endif // COLOUR_H