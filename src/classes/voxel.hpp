#ifndef VOXEL_H
#define VOXEL_H

#include "../common.hpp"
#include "colour.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <array>
#include <cstdlib>

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

        const Colour& get_colour() const noexcept {
            return colour;
        }

        bool operator==(const Voxel& other) const noexcept {
            return (colour == other.colour && opaque == other.opaque);
        }

        // bool operator!=(const Voxel& other) const noexcept {
        //     return !(*this == other);
        // }

    private:
        bool opaque = true;
        Colour colour = Colour::red();
    };
} // namespace geometry
#endif // VOXEL_H