#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "aabb.hpp"
#include "voxel.hpp"
#include <cmath>
#include <iostream>

#include <vector>

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

        explicit VoxelGrid(Vec3 centre) {
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        explicit VoxelGrid(unsigned int world_size) {
            size = Coordinate(static_cast<int>(world_size));
            initialise();
        }

        explicit VoxelGrid(unsigned int world_size, Vec3 centre) {
            size = Coordinate(static_cast<int>(world_size));
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        explicit VoxelGrid(unsigned int x, unsigned int y, unsigned int z) {
            size = Coordinate(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
            initialise();
        }

        explicit VoxelGrid(unsigned int x, unsigned int y, unsigned int z, Vec3 centre) {
            size = Coordinate(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        // TODO: Check if this returns shallow or deep copy of the Voxel.
        inline Voxel get_voxel(Coordinate coords) {
            unsigned int index = flatten(coords);
            if (static_cast<int>(index) >= size.x * size.y * size.z) {
                std::cerr << "[!] Invalid coordinates: " << coords << " -> " << index << "\n";
                return Voxel::empty();
            }

            // ! DEBUG
            // std::cout << index << "\n";

            return world.at(index);
        }

        inline Voxel get_voxel(Vec3 pos) {
            auto coords = get_voxel(get_coords(pos));
            if (!coords.is_opaque()) {
                // ! DEBUG
                std::cout << pos << "\n";
            }

            return coords;
        }

        inline Voxel get_voxel(unsigned int x, unsigned int y, unsigned int z) {
            return get_voxel(Coordinate(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)));
        }

        Coordinate get_coords(Vec3 pos) {
            if (contains(pos)) {
                int x = static_cast<int>(round_to_zero((pos.x - origin.x) / scale.x));
                int y = static_cast<int>(round_to_zero((pos.y - origin.y) / scale.y));
                int z = static_cast<int>(round_to_zero((pos.z - origin.z) / scale.z));
                // ! DEBUG
                // std::cout << pos << ", " << origin << " -> " << Coordinate(x, y, z) << "\n";
                return Coordinate(x, y, z);
            }
            else {
                // Return a coordinate that is clearly an error. We could handle this error more elegantly but
                // this is good enough for debugging purposes.
                std::cerr << pos << " is not in bounds: [" << bounding_box.min << ", " << bounding_box.max << "]\n";
                return Coordinate(std::numeric_limits<int>().max());
            }
        }

        // ! May return a position outside of the bounding box.
        // TODO: Make this properly check the coordinates are inside the grid.
        Vec3 get_pos(Coordinate coords) {
            return origin + Vec3((coords.x + 0.5) * scale.x, (coords.y + 0.5) * scale.y, (coords.z + 0.5) * scale.z);
        }

        void set_voxel(int x, int y, int z, Voxel voxel) {
            // TODO: Throw an error if the coordinates are invalid.
            world.at(flatten(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z))) =
                voxel;
        }

        void set_voxel(Coordinate coords, Voxel voxel) {
            set_voxel(coords.x, coords.y, coords.z, voxel);
        }

        bool contains(Vec3 pos) {
            return bounding_box.contains(pos);
        }

        bool contains(Coordinate coords) {
            return coords.x >= 0 && coords.x < size.x && coords.y >= 0 && coords.y < size.y && coords.z >= 0
                   && coords.z < size.z;
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
            return abs(coord1.x - coord2.x) + abs(coord1.y - coord2.y) + abs(coord1.z - coord2.z);
        }

        void create_sphere(Coordinate centre, num radius) {
            // ! DEBUG
            std::cerr << "Sphere:\n";
            std::cerr << "x: " << static_cast<int>(std::max(static_cast<num>(centre.x) - radius, 0.0)) << "-"
                      << centre.x + radius << "\n";
            std::cerr << "y: " << static_cast<int>(std::max(static_cast<num>(centre.y) - radius, 0.0)) << "-"
                      << centre.y + radius << "\n";
            std::cerr << "z: " << static_cast<int>(std::max(static_cast<num>(centre.z) - radius, 0.0)) << "-"
                      << centre.z + radius << "\n";

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
                            // ! DEBUG
                            std::cerr << Coordinate(x, y, z) << "\n";
                            set_voxel(x, y, z, Voxel());
                        }
                    }
                }
            }
        }

        void create_cube(Coordinate coords) {
            set_voxel(coords, Voxel());
        }

        void create_cube(Coordinate min, Coordinate max) {
            for (int x = std::max(min.x, 0); x <= std::min(max.x, size.x - 1); x++) {
                for (int y = std::max(min.y, 0); y <= std::min(max.y, size.y - 1); y++) {
                    for (int z = std::max(min.z, 0); z <= std::min(max.z, size.z - 1); z++) {
                        // ! DEBUG
                        // std::cerr << x << ", "<< y << ", " << z << "\n";
                        // std::cerr << get_pos(Coordinate(x, y, z)) << "\n";
                        set_voxel(x, y, z, Voxel());
                    }
                }
            }
        }

    private:
        // ! If the coordinates are too large, this may return an index outside the VoxelGrid!
        inline unsigned int flatten(Coordinate coords) {
            return flatten(static_cast<unsigned int>(coords.x),
                           static_cast<unsigned int>(coords.y),
                           static_cast<unsigned int>(coords.z));
        }

        inline unsigned int flatten(unsigned int x, unsigned int y, unsigned int z) {
            return x * static_cast<unsigned int>(size.y) * static_cast<unsigned int>(size.z)
                   + y * static_cast<unsigned int>(size.z) + z;
        }

        // ! If index is too large, this may return a coordinate outside the VoxelGrid!
        inline Coordinate unflatten(unsigned int index) {
            int x = static_cast<int>(index) / (size.y + size.z);
            int y = (static_cast<int>(index) - x * (size.y + size.z)) / size.z;
            int z = static_cast<int>(index) - x * (size.y + size.z) - y * size.z;
            return Coordinate(x, y, z);
        }

        inline void initialise() {
            // ! DEBUG
            std::cout << "   > Allocating space..."
                      << "\n";
            world.resize(static_cast<size_t>(size.x * size.y * size.z));

            // ! DEBUG
            std::cout << "   > Zeroing memory..."
                      << "\n";
            std::fill(world.begin(), world.end(), Voxel::empty());
            // ! DEBUG
            std::cout << "   > " << world.size() << " of " << world.capacity() << " bytes used"
                      << "\n";
            std::cout << "   > origin at " << origin <<"\n";

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