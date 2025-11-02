#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../common.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

namespace geometry {
    class Intersection {
    public:
        enum FaceOrientation {
            X_POS = 1,
            X_NEG = 2,
            Y_POS = 4,
            Y_NEG = 8,
            Z_POS = 16,
            Z_NEG = 32,
        };

        Voxel voxel; // for colour and material data
        num distance = 0; // for depth shader.

        explicit constexpr Intersection(Voxel voxel)
        : voxel(voxel) {}
        explicit constexpr Intersection(Voxel voxel, num distance)
        : voxel(voxel)
        , distance(distance) {}

        static constexpr Intersection invalid() {
            return Intersection(Voxel(), -1);
        }
    };

} // namespace geometry

#endif // INTERSECTION_H