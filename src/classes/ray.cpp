#include "ray.hpp"

#include "../helper.hpp"

#include <iostream>
#include <limits>

using namespace helper;

namespace geometry {
    Interval Ray::intersection(AABB bounding_box) const {
        Vec3 dummy;
        return intersection(bounding_box, dummy, dummy);
    }

    Interval Ray::intersection(AABB bounding_box, Vec3& tmin, Vec3& tmax) const {
        auto t_min = 0.0;
        auto t_max = infinity;

        auto x_min = (orientation.x().is_none()
                         ? -infinity
                         : ((orientation.is_xpos() ? bounding_box.min.x : bounding_box.max.x) - origin.x) * inv_dir.x);
        auto x_max = (orientation.x().is_none()
                         ? infinity
                         : ((orientation.is_xpos() ? bounding_box.max.x : bounding_box.min.x) - origin.x) * inv_dir.x);
        auto y_min = (orientation.y().is_none()
                         ? -infinity
                         : ((orientation.is_ypos() ? bounding_box.min.y : bounding_box.max.y) - origin.y) * inv_dir.y);
        auto y_max = (orientation.y().is_none()
                         ? infinity
                         : ((orientation.is_ypos() ? bounding_box.max.y : bounding_box.min.y) - origin.y) * inv_dir.y);
        auto z_min = (orientation.z().is_none()
                         ? -infinity
                         : ((orientation.is_zpos() ? bounding_box.min.z : bounding_box.max.z) - origin.z) * inv_dir.z);
        auto z_max = (orientation.z().is_none()
                         ? infinity
                         : ((orientation.is_zpos() ? bounding_box.max.z : bounding_box.min.z) - origin.z) * inv_dir.z);

        // ! DEBUG
        if (debug) {
            std::cerr << "x: " << Interval(x_min, x_max) << ", y: " << Interval(y_min, y_max)
                      << ", z: " << Interval(z_min, z_max) << "\n";
        }

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        tmin = Vec3(x_min, y_min, z_min);
        tmax = Vec3(x_max, y_max, z_max);

        return Interval(t_min, t_max);
    }
} // namespace geometry