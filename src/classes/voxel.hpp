#ifndef VOXEL_H
#define VOXEL_H

#include "../common.hpp"
#include "colour.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    class Voxel {
        using Faces = std::array<texture::Colour, NUM_FACES>;
        using Colour = texture::Colour;
        using Material = texture::Material;
    public:
        bool opaque = true;
        Faces colours;
        Material material = Material();

        explicit constexpr Voxel() {
            colours.fill(Colour::white());
        };

        explicit constexpr Voxel(Colour colour) {
            colours.fill(colour);
        }

        explicit constexpr Voxel(Colour face1, Colour face2, Colour face3, Colour face4, Colour face5, Colour face6) {
            colours = {face1, face2, face3, face4, face5, face6};
        }
    };
} // namespace geometry
#endif // VOXEL_H