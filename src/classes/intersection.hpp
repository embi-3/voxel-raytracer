#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../common.hpp"
#include "direction.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

namespace geometry {
    static Voxel INVALID_VOXEL = Voxel();

    class Intersection {
    public:
        std::reference_wrapper<Voxel> voxel; // for colour and material data
        num distance = 0; // for depth shader.
        Direction orientation = Direction(NONE);
        // TODO: Implement this!
        Vec3 normal;

        explicit constexpr Intersection(Voxel& voxel)
        : voxel(voxel) {}
        explicit constexpr Intersection(Voxel& voxel, num distance)
        : voxel(voxel)
        , distance(distance) {}
        explicit constexpr Intersection(Voxel& voxel, num distance, Direction orientation)
        : voxel(voxel)
        , distance(distance)
        , orientation(orientation) {}

        static constexpr Intersection invalid() {
            return Intersection(INVALID_VOXEL, -1);
        }
    };

} // namespace geometry

#endif // INTERSECTION_H