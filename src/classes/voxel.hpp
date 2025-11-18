#ifndef VOXEL_H
#define VOXEL_H

#include "../common.hpp"
#include "colour.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    using namespace texture;
    class Voxel {
    public:
        bool opaque = true;
        Colour colour = Colour::white();

        explicit constexpr Voxel() = default;

        explicit constexpr Voxel(Colour colour): colour(colour)  {}

        explicit constexpr Voxel(bool opaque)
        : opaque(opaque) {}

        static constexpr Voxel empty() {
            return Voxel(false);
        }

        constexpr bool is_opaque() {
            return opaque;
        }
    };
} // namespace geometry
#endif // VOXEL_H