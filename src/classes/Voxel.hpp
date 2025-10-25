#ifndef VOXEL_H
#define VOXEL_H

#include "Common.hpp"
#include "Vec3.hpp"
#include "Colour.hpp"
#include "Material.hpp"

#include <array>

constexpr int NUM_FACES = 6;

namespace geometry {
    class Voxel {
        public:
            geometry::Vec3 coords = geometry::Vec3();
            std::array<texture::Colour, NUM_FACES> colours = {Colour(), Colour(), Colour(), Colour(), Colour(), Colour()}; // ! This is really bad code.
            texture::Material material = Material();
            
            explicit constexpr Voxel() = default;
    };
}
#endif // VOXEL_H