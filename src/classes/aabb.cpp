#include <limits>

#include "../common.hpp"
#include "aabb.hpp"

namespace geometry {
    Interval geometry::AABB::intersection(Ray ray) {
        num t_min = 0; // ? should this be 0 instead?
        num t_max = std::numeric_limits<num>::infinity();

        // TODO: Implement precomputation of inverses.
        num x_min = (ray.origin.x - min.x) / ray.dir.x;
        num x_max = (ray.origin.x - max.x) / ray.dir.x;
        num y_min = (ray.origin.y - min.y) / ray.dir.y;
        num y_max = (ray.origin.y - max.y) / ray.dir.y;
        num z_min = (ray.origin.z - min.z) / ray.dir.z;
        num z_max = (ray.origin.z - max.z) / ray.dir.z;

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        return Interval(t_min, t_max);
    }
} // namespace geometry
