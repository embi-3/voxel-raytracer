#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../common.hpp"
#include "direction.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

namespace geometry {
    class Intersection {
    public:
        explicit Intersection(const Voxel& voxel) noexcept
        : voxel(voxel) {}

        explicit Intersection(const Voxel& voxel, num distance) noexcept
        : voxel(voxel)
        , distance(distance) {}

        explicit Intersection(const Voxel& voxel, num distance, Direction orientation) noexcept
        : voxel(voxel)
        , distance(distance)
        , orientation(orientation) {}

        static Intersection invalid() noexcept {
            return Intersection(INVALID_VOXEL, -1);
        }

        // std::reference_wrapper<const Voxel> voxel; // for colour and material data
        Voxel voxel;
        num distance = 0; // for depth shader.
        Direction orientation = Direction(NONE);
        // TODO: Implement this!
        Vec3 normal;
    };

} // namespace geometry

#endif // INTERSECTION_H