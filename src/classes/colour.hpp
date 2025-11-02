#ifndef COLOUR_H
#define COLOUR_H

#include "../common.hpp"
#include "vec3.hpp"

#include <cstdint>
// format library requires clang-17
// #include <format>

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
				a = (a + rhs.a) / 2;
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

			inline bool is_valid() {
				return (r >= 0 && r < 256)
					&& (g >= 0 && g < 256)
					&& (b >= 0 && b < 256)
					&& (a >= 0.0 && a <= 1.0);
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

	// ! These operations may produce invalid RGBA values! Be careful of overflows!
    inline Colour operator+(const Colour& u, const Colour& v) noexcept {
        Colour res = Colour(u.r + v.r, u.g + v.g, u.b + v.b, std::min((u.a) + (u.a * v.a), 1.0));
		if (!res.is_valid()) {
			std::cerr << "Invalid RGBA value! Received " << res.to_rgb_string() << "\n";
			return Colour::black();
		}

		return res;
    }

	// ! These operations may produce invalid RGBA values! Be careful of overflows!
    inline Colour operator*(const num s, const Colour& c) noexcept {
        Colour res = Colour(s*c.r, s*c.g, s*c.b, c.a);
		if (!res.is_valid()) {
			std::cerr << "Invalid RGBA value! Received " << res.to_rgba_string() << "\n";
			return Colour::black();
		}

		return res;
    }

	// ! These operations may produce invalid RGBA values! Be careful of overflows!
	inline Colour operator*(const Colour& c, const num s) noexcept {
        Colour res = Colour(s*c.r, s*c.g, s*c.b, c.a);
		if (!res.is_valid()) {
			std::cerr << "Invalid RGBA value! Received " << res.to_rgba_string() << "\n";
			return Colour::black();
		}

		return res;
    }

	// Blend Modes
	Colour normal_blend(const Colour& u, const Colour& v) noexcept;
}

#endif // COLOUR_H