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

    public:
        bool opaque = true;
        // geometry::Vec3 coords = geometry::Vec3(); // ! We shouldn't store the coordinates for the voxel in itself.
        // Faces colours = {}; // ! This is really bad code.
        // texture::Material material = Material();

        explicit constexpr Voxel() = default;
    };
} // namespace geometry
#endif // VOXEL_H