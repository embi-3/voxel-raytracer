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
                std::cerr << "[!] Invalid coordinates: " << coords << " -> " << index << "\n";
                return {};
            }

            // ! DEBUG
            // std::cout << index << "\n";
            
            return world.at(index);
        }

        inline optional<Voxel> at(Vec3 pos) {
            auto coords = at(get_coords(pos));
            if (!coords.has_value()) {
                // ! DEBUG
                std::cout << pos << "\n";
            }

            return coords;
        }

        inline optional<Voxel> at(unsigned int x, unsigned int y, unsigned int z) {
            return at(Coordinate(x, y, z));
        }

        Coordinate get_coords(Vec3 pos) {
            if (contains(pos)) {
                unsigned int x = static_cast<unsigned int>(round_to_zero((pos.x - origin.x) / scale.x));
                unsigned int y = static_cast<unsigned int>(round_to_zero((pos.y - origin.y) / scale.y));
                unsigned int z = static_cast<unsigned int>(round_to_zero((pos.z - origin.z) / scale.z));
                // ! DEBUG
                std::cout << pos << ", " << origin << " -> " << Coordinate(x, y, z) << "\n";
                return Coordinate(x, y, z);
            }
            else {
                // Return a coordinate that is clearly an error. We could handle this error more elegantly but
                // this is good enough for debugging purposes.
                return Coordinate(std::numeric_limits<unsigned int>().max());
            }
        }

        // ! May return a position outside of the bounding box.
        // TODO: Make this properly check the coordinates are inside the grid.
        Vec3 get_pos(Coordinate coords) {
            return origin + Vec3(
                (coords.x + 0.5) * scale.x,
                (coords.y + 0.5) * scale.y,
                (coords.z + 0.5) * scale.z
            );
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
            // ! DEBUG
            std::cerr << "Sphere:\n";
            std::cerr << "x: " << std::max(centre.x - radius, 0u) << "-" << centre.x + radius << "\n";
            std::cerr << "y: " << std::max(centre.y - radius, 0u) << "-" << centre.y + radius << "\n";
            std::cerr << "z: " << std::max(centre.z - radius, 0u) << "-" << centre.z + radius << "\n";

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
            // ! DEBUG
            std::cerr << "Sphere:\n";
            std::cerr << "x: " << std::max(centre.x - size / 2, 0u) << "-" << (centre.x + size) / 2 << "\n";
            std::cerr << "y: " << std::max(centre.y - size / 2, 0u) << "-" << (centre.y + size) / 2 << "\n";
            std::cerr << "z: " << std::max(centre.z - size / 2, 0u) << "-" << (centre.z + size) / 2 << "\n";

            for (unsigned int x = std::max(centre.x - size / 2, 0u); 2 * x < 2 * centre.x + size; x++) {
                for (unsigned int y = std::max(centre.y - size / 2, 0u); 2 * y < 2 * centre.y + size; y++) {
                    for (unsigned int z = std::max(centre.z - size / 2, 0u); 2 * z < 2 * centre.z + size; z++) {
                        // ! DEBUG
                        // std::cerr << x << ", "<< y << ", " << z << "\n";
                        // std::cerr << get_pos(Coordinate(x, y, z)) << "\n";
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

            Vec3 min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
            Vec3 max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }

        // Rounds negative numbers up to 0, and positive numbers down to 0.
        inline num round_to_zero(num input) {
            num integer;
            num decimal = std::modf(input, &integer);
            if (std::abs(decimal) <= 0.5) {
                return integer;
            }
            return integer + std::copysign(1, input);
        }
    };
} // namespace geometry

#endif // VOXELGRID_H