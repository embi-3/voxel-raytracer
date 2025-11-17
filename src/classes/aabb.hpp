#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "interval.hpp"
#include "vec3.hpp"

namespace geometry {
    static const num EPSILON = 1.E-10;

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
            // if (EPSILON < min.x - pos.x || pos.x - max.x > EPSILON) {
            //     std::cout << "outside x bounds by " << std::max(min.x - pos.x, pos.x - max.x) << "!\n";
            // }
            
            // if (EPSILON < min.y - pos.y || pos.y - max.y > EPSILON) {
            //     std::cout << "outside y bounds by " << std::max(min.y - pos.y, pos.y - max.y) << "!\n";
            // }
            
            // if (EPSILON < min.z - pos.z || pos.z - max.z > EPSILON) {
            //     std::cout << "outside z bounds by " << std::max(min.z - pos.z, pos.z - max.z) << "!\n";
            // }
            
            return EPSILON >= min.x - pos.x && pos.x - max.x <= EPSILON 
                && EPSILON >= min.y - pos.y && pos.y - max.y <= EPSILON 
                && EPSILON >= min.z - pos.z && pos.z - max.z <= EPSILON;
        }
    };
} // namespace geometry

#endif // AABB_H
