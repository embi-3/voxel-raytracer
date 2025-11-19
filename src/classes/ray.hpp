#ifndef RAY_H
#define RAY_H

#include "../common.hpp"
#include "../helper.hpp"
#include "intersection.hpp"
#include "intersection_list.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "voxel.hpp"
#include "voxel_grid.hpp"
#include <vector>

using namespace helper;

namespace geometry {
    class Ray {
    public:
        Vec3 origin;
        Vec3 dir;
        Direction orientation;
        Vec3 inv_dir;

        explicit constexpr Ray(Vec3 origin, Vec3 dir)
        : origin(origin)
        , dir(dir)
        , orientation(Direction(dir)) {
            inv_dir.x = equals_zero(dir.x) ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.x;
            inv_dir.y = equals_zero(dir.y) ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.y;
            inv_dir.z = equals_zero(dir.z) ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.z;
        }

        IntersectionList traverse(const VoxelGrid& grid);

        IntersectionList traverse(const Scene& scene);

        Interval intersection(AABB bounding_box);
        Interval intersection(AABB bounding_box, Vec3& tmin, Vec3& tmax);

        bool intersects(AABB bounding_box);

        inline Vec3 at(num t) {
            return origin + t * dir;
        }
    };
} // namespace geometry

#endif // RAY_H