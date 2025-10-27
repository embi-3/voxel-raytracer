#ifndef VOXEL_H
#define VOXEL_H

#include "../common.hpp"
#include "vec3.hpp"
#include "colour.hpp"
#include "material.hpp"

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    class Voxel {
        using Faces = std::array<texture::Colour, NUM_FACES>;
<<<<<<< HEAD
        public:
            bool opaque = true;
            // geometry::Vec3 coords = geometry::Vec3(); // ! We shouldn't store the coordinates for the voxel in itself.
            // Faces colours = {}; // ! This is really bad code.
            // texture::Material material = Material();
            
            explicit constexpr Voxel() = default;
=======

    public:
        geometry::Vec3 coords = geometry::Vec3();
        // Faces colours = {}; // ! This is really bad code.
        texture::Material material = texture::Material();

        explicit constexpr Voxel() = default;
>>>>>>> 12f4103 (Delete colour array default)
    };
}
#endif // VOXEL_H