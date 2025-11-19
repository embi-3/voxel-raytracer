#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "aabb.hpp"
#include "voxel.hpp"
#include "aabb.hpp"
#include <cmath>
#include <iostream>

#include <vector>

namespace geometry {
    class VoxelGrid {
    public:
        virtual ~VoxelGrid() = default;

        virtual Voxel get_voxel(Coordinate coords) const = 0;

        virtual Coordinate get_coords(Vec3 pos, Direction orientation) const = 0;

        virtual Vec3 get_pos(Coordinate coords) const = 0;

        virtual void set_voxel(Coordinate coords, Voxel voxel) = 0;

        virtual bool contains(Vec3 pos) const = 0;

        virtual bool contains(Coordinate coords) const = 0;

        virtual num space_dist(Coordinate coord1, Coordinate coord2) const = 0;

        virtual num pos_dist(Coordinate coord1, Coordinate coord2) const = 0;

        virtual num man_dist(Coordinate coord1, Coordinate coord2) const = 0;

        virtual void create_sphere(Coordinate centre, num radius) = 0;

        virtual void create_cube(Coordinate coords) = 0;

        virtual void create_cube(Coordinate min, Coordinate max) = 0;

        Vec3 scale = Vec3(1); // Size of a voxel in 3D space
        Coordinate size = Coordinate(32); // Default world dimensions
        Vec3 origin = Vec3(); // (0, 0, 0)
        AABB bounding_box; // Precompute the bounding box of the grid for performance.
    };

    class ArrayVoxelGrid : public VoxelGrid {
    public:
        explicit ArrayVoxelGrid() {
            initialise();
        }

        explicit ArrayVoxelGrid(Vec3 centre) {
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        explicit ArrayVoxelGrid(int world_size) {
            size = Coordinate(world_size);
            initialise();
        }

        explicit ArrayVoxelGrid(int world_size, Vec3 centre) {
            size = Coordinate(world_size);
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        explicit ArrayVoxelGrid(int x, int y, int z) {
            size = Coordinate(x, y, z);
            initialise();
        }

        explicit ArrayVoxelGrid(int x, int y, int z, Vec3 centre) {
            size = Coordinate(x, y, z);
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        // TODO: Check if this returns shallow or deep copy of the Voxel.
        Voxel get_voxel(Coordinate coords) const override {
            size_t index = flatten(coords);
            if (index >= static_cast<size_t>(size.x * size.y * size.z)) {
                std::cerr << "[!] Invalid coordinates for grid: " << coords << " -> " << index << "\n";
                return Voxel::empty();
            }

            return world.at(index);
        }

        // ! The orientation is used to distinguish ties for coordinates on the border of voxels.
        Coordinate get_coords(Vec3 pos, Direction orientation) const override {
            if (contains(pos)) {
                num x_scaled = (pos.x - origin.x) / scale.x;
                num y_scaled = (pos.y - origin.y) / scale.y;
                num z_scaled = (pos.z - origin.z) / scale.z;

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
            }
            else {
                // Return a coordinate that is clearly an error. We could handle this error more elegantly but
                // this is good enough for debugging purposes.
                std::cerr << "[!] " << pos << " is not in bounds: [" << bounding_box.min << ", " << bounding_box.max
                          << "]\n";
                return Coordinate(std::numeric_limits<int>().max());
            }
        }

        // ! May return a position outside of the bounding box.
        Vec3 get_pos(Coordinate coords) const override {
            return origin + Vec3(coords.x * scale.x, coords.y * scale.y, coords.z * scale.z);
        }

        void set_voxel(Coordinate coords, Voxel voxel) override {
            world.at(flatten(coords)) = voxel;
        }

        bool contains(Vec3 pos) const override {
            return bounding_box.contains(pos);
        }

        bool contains(Coordinate coords) const override {
            return coords.x >= 0 && coords.x < size.x && coords.y >= 0 && coords.y < size.y && coords.z >= 0
                   && coords.z < size.z;
        }

        // Returns the distance between the centres of two voxels in 3D space, including scaling.
        num space_dist(Coordinate coord1, Coordinate coord2) const override {
            return sqrt(pow((coord1.x - coord2.x) * scale.x, 2) + pow((coord1.y - coord2.y) * scale.y, 2)
                        + pow((coord1.z - coord2.z) * scale.z, 2));
        }

        // Returns the distance between the centres of two voxels in grid space, ignoring scaling.
        num pos_dist(Coordinate coord1, Coordinate coord2) const override {
            return sqrt(pow(coord1.x - coord2.x, 2) + pow(coord1.y - coord2.y, 2) + pow(coord1.z - coord2.z, 2));
        }

        // Returns the Manhattan (taxicab) distance between two voxels.
        num man_dist(Coordinate coord1, Coordinate coord2) const override {
            return abs(coord1.x - coord2.x) + abs(coord1.y - coord2.y) + abs(coord1.z - coord2.z);
        }

        void create_sphere(Coordinate centre, num radius) override {
            for (int x = static_cast<int>(std::max(static_cast<num>(centre.x) - radius, 0.0));
                 x <= centre.x + radius && x < size.x;
                 x++)
            {
                for (int y = static_cast<int>(std::max(static_cast<num>(centre.y) - radius, 0.0));
                     y <= centre.y + radius && x < size.y;
                     y++)
                {
                    for (int z = static_cast<int>(std::max(static_cast<num>(centre.z) - radius, 0.0));
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

        void create_cube(Coordinate coords) override {
            set_voxel(coords, Voxel());
        }

        void create_cube(Coordinate min, Coordinate max) override {
            for (int x = std::max(min.x, 0); x <= std::min(max.x, size.x - 1); x++) {
                for (int y = std::max(min.y, 0); y <= std::min(max.y, size.y - 1); y++) {
                    for (int z = std::max(min.z, 0); z <= std::min(max.z, size.z - 1); z++) {
                        set_voxel(Coordinate(x, y, z), Voxel());
                    }
                }
            }
        }

    private:
        std::vector<Voxel> world = {}; // 1D array representation of the voxel world

        // ! If the coordinates are too large, this may return an index outside the VoxelGrid!
        size_t flatten(Coordinate coords) const {
            return static_cast<size_t>(coords.x * size.y * size.z + coords.y * size.z + coords.z);
        }

        // ! If index is too large, this may return a coordinate outside the VoxelGrid!
        Coordinate unflatten(size_t index) const {
            int x = static_cast<int>(index) / (size.y + size.z);
            int y = (static_cast<int>(index) - x * (size.y + size.z)) / size.z;
            int z = static_cast<int>(index) - x * (size.y + size.z) - y * size.z;
            return Coordinate(x, y, z);
        }

        void initialise() {
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

            Vec3 min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
            Vec3 max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }
    };
} // namespace geometry

#endif // VOXELGRID_H