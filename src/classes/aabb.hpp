#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "interval.hpp"
#include "vec3.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
    public:
        Vec3 min;
        Vec3 max;

        explicit constexpr AABB() = default;
        explicit constexpr AABB(Vec3 min, Vec3 max)
        : min(min)
        , max(max){};

        bool contains(Vec3 pos) {
            // ! DEBUG
            // if (epsilon < min.x - pos.x || pos.x - max.x > epsilon) {
            //     std::cout << "outside x bounds by " << std::max(min.x - pos.x, pos.x - max.x) << "!\n";
            // }
            
            // if (epsilon < min.y - pos.y || pos.y - max.y > epsilon) {
            //     std::cout << "outside y bounds by " << std::max(min.y - pos.y, pos.y - max.y) << "!\n";
            // }
            
            // if (epsilon < min.z - pos.z || pos.z - max.z > epsilon) {
            //     std::cout << "outside z bounds by " << std::max(min.z - pos.z, pos.z - max.z) << "!\n";
            // }
            
            return epsilon >= min.x - pos.x && pos.x - max.x <= epsilon 
                && epsilon >= min.y - pos.y && pos.y - max.y <= epsilon 
                && epsilon >= min.z - pos.z && pos.z - max.z <= epsilon;
        }
    };
} // namespace geometry

#endif // AABB_H
