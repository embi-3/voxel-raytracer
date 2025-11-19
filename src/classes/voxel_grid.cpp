#include "ray.hpp"
#include "voxel_grid.hpp"

#include <stack>

namespace geometry {

// ========================================================================
//  VoxelGrid
// ========================================================================

    // ! The orientation is used to distinguish ties for coordinates on the border of voxels.
    Coordinate VoxelGrid::get_coords(Vec3 pos, Direction orientation) const {
        if (contains(pos)) {
            auto x_scaled = (pos.x - origin.x) / scale.x;
            auto y_scaled = (pos.y - origin.y) / scale.y;
            auto z_scaled = (pos.z - origin.z) / scale.z;

            if (std::abs(x_scaled - 0.5 - floor(x_scaled - 0.5)) < epsilon) {
                x_scaled = (orientation.is_xpos() ? ceil(x_scaled) : floor(x_scaled));
            }

            if (std::abs(y_scaled - 0.5 - floor(y_scaled - 0.5)) < epsilon) {
                y_scaled = (orientation.is_ypos() ? ceil(y_scaled) : floor(y_scaled));
            }

            if (std::abs(z_scaled - 0.5 - floor(z_scaled - 0.5)) < epsilon) {
                z_scaled = (orientation.is_zpos() ? ceil(z_scaled) : floor(z_scaled));
            }

            return Coordinate(static_cast<int>(round(x_scaled)),
                                static_cast<int>(round(y_scaled)),
                                static_cast<int>(round(z_scaled)));
        } else {
            auto stream = StringStream{};
            stream << "[!] " << pos << "is not in bounds: " << bounding_box << "\n";
            throw std::invalid_argument(stream.str());
        }
    }

    void VoxelGrid::create_cube(Coordinate min, Coordinate max) {
        for (auto x = std::max(min.x, 0); x <= std::min(max.x, size.x - 1); x++) {
            for (auto y = std::max(min.y, 0); y <= std::min(max.y, size.y - 1); y++) {
                for (auto z = std::max(min.z, 0); z <= std::min(max.z, size.z - 1); z++) {
                    set_voxel(Coordinate(x, y, z), Voxel());
                }
            }
        }
    }

    void VoxelGrid::create_sphere(Coordinate centre, num radius) {
        for (auto x = static_cast<int>(std::max(static_cast<num>(centre.x) - radius, 0.0));
                x <= centre.x + radius && x < size.x;
                x++)
        {
            for (auto y = static_cast<int>(std::max(static_cast<num>(centre.y) - radius, 0.0));
                    y <= centre.y + radius && x < size.y;
                    y++)
            {
                for (auto z = static_cast<int>(std::max(static_cast<num>(centre.z) - radius, 0.0));
                        z <= centre.z + radius && x < size.z;
                        z++)
                {
                    if (space_dist(centre, Coordinate(x, y, z)) <= radius) {
                        set_voxel(Coordinate(x, y, z), Voxel());
                    }
                }
            }
        }
    }

// ========================================================================
//  ArrayVoxelGrid
// ========================================================================

    IntersectionList ArrayVoxelGrid::traverse(Ray ray) const {
        IntersectionList objects = IntersectionList(ray.dir);

        Vec3 tmax;
        Vec3 tdelta = Vec3(equals_zero(ray.dir.x) ? infinity : fabs(scale.x * ray.inv_dir.x),
                        equals_zero(ray.dir.y) ? infinity : fabs(scale.y * ray.inv_dir.y),
                        equals_zero(ray.dir.z) ? infinity : fabs(scale.z * ray.inv_dir.z));
        num tcur = 0;
        Direction normal = Direction();
        Coordinate step = ray.orientation.sign();
        Coordinate coords;

        Vec3 min;
        Vec3 max;
        Interval interval = ray.intersection(bounding_box, min, max);
        Voxel voxel;

        // ! DEBUG
        if (debug) {
            std::cerr << "[t] Interval: [" << interval.min << ", " << interval.max << "]\n";
            std::cerr << "[t] Delta: " << tdelta << "\n";
        }

        if (interval.is_valid()) {
            tcur = interval.min;

            // Calculate the first t which intersects with each coordinate plane.
            // If it's infinity, don't bother.
            // ! Double check the order of multiplications / divisions is correct!
            num first_x =
                (max.x == infinity
                    ? infinity
                    : std::max(
                        min.x,
                        max.x - (trunc((max.x - interval.min) * ray.dir.x / scale.x) * scale.x * ray.inv_dir.x)));
            num first_y =
                (max.y == infinity
                    ? infinity
                    : std::max(
                        min.y,
                        max.y - (trunc((max.y - interval.min) * ray.dir.y / scale.y) * scale.y * ray.inv_dir.y)));
            num first_z =
                (max.z == infinity
                    ? infinity
                    : std::max(
                        min.z,
                        max.z - (trunc((max.z - interval.min) * ray.dir.z / scale.z) * scale.z * ray.inv_dir.z)));

            tmax = Vec3(first_x, first_y, first_z);

            // ! DEBUG
            if (debug) {
                std::cerr << "pos: " << ray.at(tcur) << ", tcur: " << tcur << ", tmax: " << tmax << "\n";
            }

            if (equals(tmax.x, tcur)) {
                // normal += orientation.x();
                tmax.x += tdelta.x;
            }
            if (equals(tmax.y, tcur)) {
                // normal += orientation.y();
                tmax.y += tdelta.y;
            }
            if (equals(tmax.z, tcur)) {
                // normal += orientation.z();
                tmax.z += tdelta.z;
            }
        }
        else {
            // If the ray doesn't hit the bounding box, return an empty list.
            return objects;
        }

        coords = get_coords(ray.at(tcur), ray.orientation);

        // ! DEBUG
        if (debug) {
            std::cerr << "Starting coords: " << coords << "\n";
        }

        // Iteratively find the next voxel using floating-point comparisons.
        while (contains(coords)) {
            // ! DEBUG
            if (debug) {
                std::cerr << "[c] " << coords << ": [" << tcur << "] " << ray.at(tcur) << " " << normal << " => "
                        << get_coords(ray.at(tcur), ray.orientation) << "\n";
            }

            voxel = get_voxel(coords);
            if (voxel.is_opaque()) {
                // ! DEBUG
                if (debug) {
                    std::cerr << "[i] ray: " << ray.dir << ", dist: " << tcur * ray.dir.length() << ", pos: " << ray.at(tcur)
                            << ", coords: " << get_coords(ray.at(tcur), ray.orientation) << ", normal: " << normal
                            << "\n";
                }

                objects.push_back(Intersection(voxel, tcur * ray.dir.length(), normal));

                // ! TEMP
                break;
            }

            // Create a temporary variable so any traversal updates don't affect the current iteration.
            Vec3 tmax_temp = tmax;

            // ! DEBUG
            if (debug) {
                std::cerr << "tmax: " << tmax_temp << "\n";
            }

            normal = Direction(NONE);

            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax_temp.x <= tmax_temp.y && tmax_temp.x <= tmax_temp.z) {
                tcur = tmax_temp.x;
                tmax.x += tdelta.x;
                coords.x += step.x;
                normal += ray.orientation.x();
            }

            if (tmax_temp.y <= tmax_temp.x && tmax_temp.y <= tmax_temp.z) {
                tcur = tmax_temp.y;
                tmax.y += tdelta.y;
                coords.y += step.y;
                normal += ray.orientation.y();
            }

            if (tmax_temp.z <= tmax_temp.x && tmax_temp.z <= tmax_temp.y) {
                tcur = tmax_temp.z;
                tmax.z += tdelta.z;
                coords.z += step.z;
                normal += ray.orientation.z();
            }
        }

        return objects;
    }

    void ArrayVoxelGrid::initialise() {
        // ! INFO
        std::cout << "   > Allocating space..."
                    << "\n";

        std::cout << "   > " << static_cast<unsigned long>(size.x * size.y * size.z) * sizeof(Voxel)
                    << " bytes required (" << sizeof(Voxel) << " bytes per voxel).\n";

        world.resize(static_cast<size_t>(size.x * size.y * size.z));

        // ! INFO
        std::cout << "   > Zeroing memory..."
                    << "\n";
        std::fill(world.begin(), world.end(), Voxel::empty());
        // ! INFO
        // std::cout << "   > " << world.size() << " of " << world.capacity() << " bytes used" << "\n";
        std::cout << "   > Origin at " << origin << "\n";

        const auto min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
        const auto max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
        bounding_box = AABB(min_bounds, max_bounds);
    }

// ========================================================================
//  SVOVoxelGrid
// ========================================================================

    const Voxel& SVOVoxelGrid::get_voxel(Coordinate coords) const {
        if (coords.x < 0 || coords.x >= size.x || coords.y < 0 || coords.y >= size.y || coords.z < 0 || coords.z >= size.z) {
            auto stream = StringStream{};
            stream << "[!] Coordinates out of bounds: " << coords << "\n";
            throw std::invalid_argument(stream.str());
        }

        const auto* curr = root.get();

        for (std::size_t depth = 0; depth < max_depth && !curr->isLeaf; depth++) {
            const auto x_bit = (static_cast<std::size_t>(coords.x) >> (max_depth - depth - 1)) & 1u;
            const auto y_bit = (static_cast<std::size_t>(coords.y) >> (max_depth - depth - 1)) & 1u;
            const auto z_bit = (static_cast<std::size_t>(coords.z) >> (max_depth - depth - 1)) & 1u;
            const auto child_index = (x_bit << 2) | (y_bit << 1) | z_bit;
            
            const auto& child = curr->children[child_index];
            if (!child) {
                return curr->data;
            }
            curr = child.get();
        }

        return curr->data;
    }

    void SVOVoxelGrid::set_voxel(Coordinate coords, Voxel voxel) {
        if (coords.x < 0 || coords.x >= size.x || coords.y < 0 || coords.y >= size.y || coords.z < 0 || coords.z >= size.z) {
            auto stream = StringStream{};
            stream << "[!] Coordinates out of bounds: " << coords << "\n";
            throw std::invalid_argument(stream.str());
        }

        auto* curr = root.get();

        for (std::size_t depth = 0; depth < max_depth; depth++) {
            const auto x_bit = (static_cast<std::size_t>(coords.x) >> (max_depth - depth - 1)) & 1u;
            const auto y_bit = (static_cast<std::size_t>(coords.y) >> (max_depth - depth - 1)) & 1u;
            const auto z_bit = (static_cast<std::size_t>(coords.z) >> (max_depth - depth - 1)) & 1u;
            const auto child_index = (x_bit << 2) | (y_bit << 1) | z_bit;
            
            auto& child = curr->children[child_index];
            if (!child) {
                auto x_dist = curr->bounding_box.max.x - curr->bounding_box.min.x;
                auto y_dist = curr->bounding_box.max.y - curr->bounding_box.min.y;
                auto z_dist = curr->bounding_box.max.z - curr->bounding_box.min.z;

                auto min = Vec3(
                    curr->bounding_box.min.x + static_cast<double>(x_bit) * (x_dist / 2),
                    curr->bounding_box.min.y + static_cast<double>(y_bit) * (y_dist / 2),
                    curr->bounding_box.min.z + static_cast<double>(z_bit) * (z_dist / 2)
                );

                auto max = min + Vec3(x_dist / 2, y_dist / 2, z_dist / 2);

                child = std::make_unique<Node>(AABB(min, max));
            }
            curr = child.get();
        }

        curr->data = voxel;
        curr->isLeaf = true;
    }

    IntersectionList SVOVoxelGrid::traverse(Ray ray) const {
        auto objects = IntersectionList(ray.dir);

        auto root_interval = ray.intersection(root->bounding_box);
        // If ray doesn't hit root bounding_box, skip
        if (!root_interval.is_valid())
            return objects;

        // Stores nodes with t-intervals
        struct StackNode {
            Node* ptr;
            num tmin;
            num tmax;
        };

        // Stack of nodes while traversing
        auto stack = std::stack<StackNode>{};
        stack.push({root.get(), root_interval.min, root_interval.max});

        // Iterate until stack is empty or opaque node is hit
        while (!stack.empty()) {
            /// Pop stack
            auto stack_node = stack.top();
            auto node = stack_node.ptr;
            stack.pop();

            if (node->isLeaf) {
                auto hit = ray.at(stack_node.tmin);
                auto node_bb = node->bounding_box;
                auto normal = Vec3();

                if (std::fabs(hit.x - node_bb.min.x) < epsilon) 
                    normal = Vec3(1, 0, 0);
                else if (std::fabs(hit.x - node_bb.max.x) < epsilon) 
                    normal = Vec3(-1, 0, 0);
                else if (std::fabs(hit.y - node_bb.min.y) < epsilon) 
                    normal = Vec3(0, 1, 0);
                else if (std::fabs(hit.y - node_bb.max.y) < epsilon)
                    normal = Vec3(0, -1, 0);
                else if (std::fabs(hit.z - node_bb.min.z) < epsilon)
                    normal = Vec3(0, 0, 1);
                else if (std::fabs(hit.z - node_bb.max.z) < epsilon)
                    normal = Vec3(0, 0, -1);

                objects.push_back(Intersection(node->data, stack_node.tmin * ray.dir.length(), normal));
                break;
            }

            auto sorted_children = std::array<StackNode, 8>{};
            std::size_t child_count = 0;
            for (std::size_t i = 0; i < 8; i++) {
                // If child is nullptr, skip
                if (!node->children[i]) continue;
                auto child = node->children[i].get();
                
                // If ray doesn't intersect, skip
                auto child_interval = ray.intersection(child->bounding_box);
                if (!child_interval.is_valid()) continue;

                // If ray doesnt originate from the parent, skip  
                num tmin = std::max(child_interval.min, stack_node.tmin); 
                num tmax = std::min(child_interval.max, stack_node.tmax); 
                if (tmin > tmax) continue;
                
                sorted_children[child_count++] = {child, tmin, tmax};
            }

            // Sort children in descending tmin order
            std::sort(
                sorted_children.begin(), 
                sorted_children.end(),
                [](const StackNode& a, const StackNode& b) { return a.tmin > b.tmin; }
            );

            for (const auto& child : sorted_children) {
                if (!child.ptr) 
                    break;

                stack.push(child);
            }
        }

        return objects;
    }

};
