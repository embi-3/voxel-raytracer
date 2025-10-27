#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Common.hpp"
#include "Vec3.hpp"
#include "Material.hpp"
#include "Voxel.hpp"

namespace geometry {
    class Intersection {
        public:
            Voxel voxel;        // for colour and material data
            num distance = 0;   // for depth shader.

        explicit constexpr Intersection(Voxel voxel) : voxel(voxel) {}
        explicit constexpr Intersection(Voxel voxel, num distance) : voxel(voxel), distance(distance) {}
    };


}

#endif // INTERSECTION_H