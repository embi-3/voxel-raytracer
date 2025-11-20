#ifndef INTERSECTION_LIST_H
#define INTERSECTION_LIST_H

#include "../common.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "voxel.hpp"

#include <vector>

namespace geometry {
    using IntersectionIterator = std::vector<Intersection>::const_iterator;

    class IntersectionList {
    public:
        explicit IntersectionList(Vec3 ray)
        : ray_dir(ray) {}

        void push_back(Intersection intersection) {
            items.push_back(intersection);
        }

        // Wrapper functions for the internal vector.
        template<typename InputIterator>
        void insert(IntersectionIterator iterator, InputIterator first, InputIterator last) {
            items.insert(iterator, first, last);
        }

        auto at(size_t index) const {
            return items.at(index);
        }

        auto begin() const noexcept {
            return items.begin();
        }

        auto end() const noexcept {
            return items.end();
        }

        auto empty() const noexcept {
            return items.empty();
        }

        auto size() const noexcept {
            return items.size();
        }

        auto ray_direction() const {
            return ray_dir.normalise();
        }
        // Moved items to public so FlatShader can access it 
        std::vector<Intersection> items;
    private:
        Vec3 ray_dir;
    };

} // namespace geometry

#endif // INTERSECTION_LIST_H