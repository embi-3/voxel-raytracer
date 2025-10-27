#ifndef RAY_H
#define RAY_H

#include "../common.hpp"
#include "intersection.hpp"
#include "vec3.hpp"
#include "voxel.hpp"
#include "voxel_grid.hpp"

namespace geometry {
    class Ray {
    public:
        Ray () {}
        Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

        const Point3& origin() const { return orig; }
        const Vec3& direction() const {return dir; }

        Point3 at(double t) const {
            return orig + t*dir;
        }

    private:
        Point3 orig;
        Vec3 dir;
    };
} // namespace geometry
#endif // RAY_H