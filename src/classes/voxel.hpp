#ifndef VOXEL_H
#define VOXEL_H

<<<<<<< HEAD
#include "../common.hpp"
#include "vec3.hpp"
#include "colour.hpp"
#include "material.hpp"
=======
#include "colour.hpp"
#include "../common.hpp"
#include "material.hpp"
#include "vec3.hpp"
>>>>>>> main

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    class Voxel {
        public:
            bool opaque = true;
            // geometry::Vec3 coords = geometry::Vec3(); // ! We shouldn't store the coordinates for the voxel in itself.
            // std::array<texture::Colour, NUM_FACES> colours = {Colour(), Colour(), Colour(), Colour(), Colour(), Colour()}; // ! This is really bad code.
            // texture::Material material = Material();
            
            explicit constexpr Voxel() = default;
    };
}
#endif // VOXEL_H