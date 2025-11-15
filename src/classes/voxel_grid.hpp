#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "voxel.hpp"
#include "aabb.hpp"
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
        std::vector<Voxel> world;

        // Precompute the bounding box of the grid for performance.
        AABB bounding_box;

        explicit VoxelGrid() {
            world.reserve(size.x * size.y * size.z); // TODO: Change the way voxels are stored.
            
            Vec3 min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
            Vec3 max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }

        explicit VoxelGrid(unsigned int world_size) {
            size = Coordinate(world_size);
            VoxelGrid();
        }

        explicit VoxelGrid(unsigned int x, unsigned int y, unsigned int z) {
            size = Coordinate(x, y, z);
            VoxelGrid();
        }

        // TODO: Check if this returns shallow or deep copy of the Voxel.d
        Voxel get_voxel(Coordinate coords) {
            unsigned int index = flatten(coords);
            return world.at(index);
        }

        Voxel get_voxel(Vec3 pos) {
            return get_voxel(get_coords(pos));
        }

        Coordinate get_coords(Vec3 pos) {
            if (contains(pos)) {
                unsigned int x = std::round(pos.x);
                unsigned int y = std::round(pos.y);
                unsigned int z = std::round(pos.z);
                return Coordinate(x, y, z);
            } else {
                // Return a coordinate that is clearly an error. We could handle this error more elegantly but
                // this is good enough for debugging purposes.
                return Coordinate(std::numeric_limits<unsigned int>().max());
            }
        }

        void set_voxel([[maybe_unused]] unsigned int x, [[maybe_unused]] unsigned int y, [[maybe_unused]] unsigned int z) {
            // Throw an error if the coordinates are invalid.
        }

        bool contains(Vec3 pos) {
            return pos.x - origin.x <= (size.x - 0.5) * scale.x
                && pos.y - origin.y <= (size.y - 0.5) * scale.y
                && pos.z - origin.z <= (size.z - 0.5) * scale.z
                && origin.x - pos.x >= 0.5 * scale.x
                && origin.y - pos.y >= 0.5 * scale.y
                && origin.z - pos.z >= 0.5 * scale.z;
        }

        bool contains(Coordinate coords) {
            return coords.x < size.x && coords.y < size.y && coords.z < size.z;
        }
    
    private:
        // ! If the coordinates are too large, this may return an index outside the VoxelGrid!
        unsigned int flatten(Coordinate coords) {
            return coords.x * size.x + coords.y * size.y + coords.z * size.z;
        }

        // ! If index is too large, this may return a coordinate outside the VoxelGrid!
        Coordinate unflatten(unsigned int index) {            
            unsigned int x = index / (size.y + size.z);
            unsigned int y = (index - x * (size.y + size.z)) / size.z;
            unsigned int z = index - x * (size.y + size.z) - y * size.z;
            return Coordinate(x, y, z);
        }
    };
} // namespace geometry

#endif // VOXELGRID_H