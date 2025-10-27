#ifndef VOXEL_H
#define VOXEL_H

#include "colour.hpp"
#include "../common.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    class Voxel {
        using Faces = std::array<texture::Colour, NUM_FACES>;

    public:
        geometry::Vec3 coords = geometry::Vec3();
        // Faces colours = {}; // ! This is really bad code.
        texture::Material material = texture::Material();

        explicit constexpr Voxel() = default;
    };
} // namespace geometry
#endif // VOXEL_H