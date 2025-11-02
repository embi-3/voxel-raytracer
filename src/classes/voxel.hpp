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
<<<<<<< HEAD
        public:
            bool opaque = true;
            // geometry::Vec3 coords = geometry::Vec3(); // ! We shouldn't store the coordinates for the voxel in itself.
            // std::array<texture::Colour, NUM_FACES> colours = {Colour(), Colour(), Colour(), Colour(), Colour(), Colour()}; // ! This is really bad code.
=======
        using Faces = std::array<texture::Colour, NUM_FACES>;
        public:
            bool opaque = true;
            // geometry::Vec3 coords = geometry::Vec3(); // ! We shouldn't store the coordinates for the voxel in itself.
            // Faces colours = {}; // ! This is really bad code.
>>>>>>> 3bc441ef3c00a390c0483cc86a8123424795139a
            // texture::Material material = Material();
            
            explicit constexpr Voxel() = default;
    };
}
#endif // VOXEL_H