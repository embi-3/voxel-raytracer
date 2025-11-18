#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../common.hpp"
#include "direction.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

namespace geometry {
    static const Voxel& INVALID_VOXEL = Voxel();
    
    class Intersection {
    public:
        const Voxel voxel; // for colour and material data
        const num distance = 0; // for depth shader.
        const Direction orientation = Direction(NONE);
        // TODO: Implement this!
        const Vec3 normal;

        explicit constexpr Intersection(const Voxel voxel)
        : voxel(voxel) {}
        explicit constexpr Intersection(const Voxel voxel, num distance)
        : voxel(voxel)
        , distance(distance) {}
        explicit constexpr Intersection(const Voxel voxel, num distance, Direction orientation)
        : voxel(voxel)
        , distance(distance)
        , orientation(orientation) {}

        static constexpr Intersection invalid() {
            return Intersection(geometry::INVALID_VOXEL, -1);
        }
    };

} // namespace geometry

#endif // INTERSECTION_H