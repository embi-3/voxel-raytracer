#ifndef INTERSECTION_LIST_H
#define INTERSECTION_LIST_H

#include "../common.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

#include <vector>

namespace geometry {
    class IntersectionList {
    public:
        Vec3 ray_dir;
        std::vector<Intersection> items;

        explicit IntersectionList(Vec3 ray)
        : ray_dir(ray) {}

        void push_back(Intersection intersection) {
            items.push_back(intersection);
        }

        // Wrapper functions for the internal vector.
        void insert(auto iterator, auto first, auto last) {
            items.insert(iterator, first, last);
        }

        auto begin() {
            return items.begin();
        }

        auto end() {
            return items.end();
        }

        auto empty() {
            return items.empty();
        }
    };

} // namespace geometry

#endif // INTERSECTION_LIST_H