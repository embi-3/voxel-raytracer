#include "voxel_grid.hpp"

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
            // Return a coordinate that is clearly an error. We could handle this error more elegantly but
            // this is good enough for debugging purposes.
            std::cerr << "[!] " << pos << " is not in bounds: [" << bounding_box.min << ", " << bounding_box.max
                        << "]\n";
            return Coordinate(std::numeric_limits<int>().max());
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
        if (coords.x >= size.x || coords.y >= size.y || coords.z >= size.z) {
            throw std::invalid_argument("Coordinates out of bounds.");
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
        if (coords.x >= size.x || coords.y >= size.y || coords.z >= size.z) {
            std::cerr << "[!] Out of bounds: " << coords << "\n";
            return;
        }

        auto* curr = root.get();

        for (std::size_t depth = 0; depth < max_depth; depth++) {
            const auto x_bit = (static_cast<std::size_t>(coords.x) >> (max_depth - depth - 1)) & 1u;
            const auto y_bit = (static_cast<std::size_t>(coords.y) >> (max_depth - depth - 1)) & 1u;
            const auto z_bit = (static_cast<std::size_t>(coords.z) >> (max_depth - depth - 1)) & 1u;
            const auto child_index = (x_bit << 2) | (y_bit << 1) | z_bit;
            
            auto& child = curr->children[child_index];
            if (!child) {
                child = std::make_unique<Node>();
            }
            curr = child.get();
        }

        curr->data = voxel;
        curr->isLeaf = true;
    }

};
