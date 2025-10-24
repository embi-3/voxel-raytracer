#ifndef VOXEL_H
#define VOXEL_H

#include "Common.hpp"
#include "Vec3.hpp"
#include "Colour.hpp"
#include "Material.hpp"

#define NUM_FACES 6

class Voxel {
    public:
        Vec3 coords = Vec3();
        Colour colours[NUM_FACES] = {Colour(), Colour(), Colour(), Colour(), Colour(), Colour()}; // ! This is really bad code.
        Material material = Material();
        
        explicit constexpr Voxel() = default;
};

#endif // VOXEL_H