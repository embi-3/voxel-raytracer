#ifndef VOXEL_H
#define VOXEL_H

#include "../common.hpp"
#include "colour.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <array>

static constexpr int NUM_FACES = 6;

namespace geometry {
    using namespace texture;

    class Voxel {
    public:
        explicit constexpr Voxel() = default;

        explicit constexpr Voxel(Colour colour) noexcept
        : colour(colour) {}

        explicit constexpr Voxel(bool opaque) noexcept
        : opaque(opaque) {}

        static constexpr Voxel empty() noexcept {
            return Voxel(false);
        }

        constexpr bool is_opaque() const noexcept {
            return opaque;
        }

        Colour get_colour() const noexcept {
            return colour;
        }

    private:
        bool opaque = true;
        Colour colour = Colour::white();
    };
} // namespace geometry
#endif // VOXEL_H