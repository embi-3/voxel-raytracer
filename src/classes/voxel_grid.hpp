#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "aabb.hpp"
#include "voxel.hpp"
#include <cmath>
#include <iostream>

#include <vector>

#define MAX_WORLD_SIZE 100

namespace geometry {
    class VoxelGrid {
    public:
        Vec3 scale = Vec3(1); // Size of a voxel in 3D space
        Coordinate size = Coordinate(32); // Default world dimensions

        Vec3 origin = Vec3();
        std::vector<Voxel> world = {};

        // Precompute the bounding box of the grid for performance.
        AABB bounding_box;

        explicit VoxelGrid() {
            initialise();
        }

        explicit VoxelGrid(Vec3 origin)
        : origin(origin) {
            initialise();
        }

        explicit VoxelGrid(unsigned int world_size, Vec3 pos = Vec3()) {
            size = Coordinate(world_size);
            origin = pos;
            initialise();
        }

        explicit VoxelGrid(unsigned int x, unsigned int y, unsigned int z, Vec3 origin = Vec3())
        : origin(origin) {
            size = Coordinate(x, y, z);
            initialise();
        }

        // TODO: Check if this returns shallow or deep copy of the Voxel.
        inline optional<Voxel> at(Coordinate coords) {
            unsigned int index = flatten(coords);
            if (index >= size.x * size.y * size.z) {
                std::cerr << "[!] Invalid index: " << index << "\n";
                return {};
            }

            // ! DEBUG
            // std::cout << index << "\n";
            
            return world.at(index);
        }

        inline optional<Voxel> at(Vec3 pos) {
            return at(get_coords(pos));
        }

        inline optional<Voxel> at(unsigned int x, unsigned int y, unsigned int z) {
            return at(Coordinate(x, y, z));
        }

        Coordinate get_coords(Vec3 pos) {
            if (contains(pos)) {
                unsigned int x = static_cast<unsigned int>(std::round(pos.x / scale.x));
                unsigned int y = static_cast<unsigned int>(std::round(pos.y / scale.y));
                unsigned int z = static_cast<unsigned int>(std::round(pos.z / scale.z));
                return Coordinate(x, y, z);
            }
            else {
                // Return a coordinate that is clearly an error. We could handle this error more elegantly but
                // this is good enough for debugging purposes.
                return Coordinate(std::numeric_limits<unsigned int>().max());
            }
        }

        // TODO: Remove this if at does the same thing.
        void set_voxel([[maybe_unused]] unsigned int x, [[maybe_unused]] unsigned int y, [[maybe_unused]] unsigned int z) {
            // Throw an error if the coordinates are invalid.
        }

        bool contains(Vec3 pos) {
            return bounding_box.contains(pos);
        }

        bool contains(Coordinate coords) {
            return coords.x < size.x && coords.y < size.y && coords.z < size.z;
        }

        // Returns the distance between the centres of two voxels in 3D space, including scaling.
        num space_dist(Coordinate coord1, Coordinate coord2) {
            return sqrt(pow((coord1.x - coord2.x) * scale.x, 2) + pow((coord1.y - coord2.y) * scale.y, 2)
                        + pow((coord1.z - coord2.z) * scale.z, 2));
        }

        // Returns the distance between the centres of two voxels in grid space, ignoring scaling.
        num pos_dist(Coordinate coord1, Coordinate coord2) {
            return sqrt(pow(coord1.x - coord2.x, 2) + pow(coord1.y - coord2.y, 2) + pow(coord1.z - coord2.z, 2));
        }

        // Returns the Manhattan (taxicab) distance between two voxels.
        num man_dist(Coordinate coord1, Coordinate coord2) {
            return abs(static_cast<int>(coord1.x - coord2.x)) + abs(static_cast<int>(coord1.y - coord2.y)) + abs(static_cast<int>(coord1.z - coord2.z));
        }

        void create_sphere(Coordinate centre, unsigned int radius) {
            for (unsigned int x = std::max(centre.x - radius, 0u); x < centre.x + radius && x < size.x; x++) {
                for (unsigned int y = std::max(centre.y - radius, 0u); y < centre.y + radius && x < size.y; y++) {
                    for (unsigned int z = std::max(centre.z - radius, 0u); z < centre.z + radius && x < size.z; z++) {
                        if (space_dist(centre, Coordinate(x, y, z)) <= radius) {
                            at(x, y, z) = Voxel();
                        }
                    }
                }
            }
        }

        void create_cube(Coordinate centre, unsigned int size) {
            for (unsigned int x = std::max(centre.x - size / 2, 0u); 2 * x < 2 * centre.x + size; x++) {
                for (unsigned int y = std::max(centre.y - size / 2, 0u); 2 * y < 2 * centre.y + size; y++) {
                    for (unsigned int z = std::max(centre.z - size / 2, 0u); 2 * z < 2 * centre.z + size; z++) {
                        // ! DEBUG
                        // std::cout << x << ", "<< y << ", " << z << "\n";
                        at(x, y, z) = Voxel();
                    }
                }
            }
        }

    private:
        // ! If the coordinates are too large, this may return an index outside the VoxelGrid!
        inline unsigned int flatten(Coordinate coords) {
            return flatten(coords.x, coords.y, coords.z);
        }

        inline unsigned int flatten(unsigned int x, unsigned int y, unsigned int z) {
            return x * size.y * size.z + y * size.z + z;
        }

        // ! If index is too large, this may return a coordinate outside the VoxelGrid!
        inline Coordinate unflatten(unsigned int index) {
            unsigned int x = index / (size.y + size.z);
            unsigned int y = (index - x * (size.y + size.z)) / size.z;
            unsigned int z = index - x * (size.y + size.z) - y * size.z;
            return Coordinate(x, y, z);
        }

        inline void initialise() {
            world.resize(size.x * size.y * size.z);
            // ! DEBUG
            std::cout << "  > " << world.size() << " of " << world.capacity() << " bytes used" << "\n";

            Vec3 min_bounds = origin;
            Vec3 max_bounds = origin + Vec3(size.x * scale.x, size.y * scale.y, size.z * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }
    };
} // namespace geometry

#endif // VOXELGRID_H