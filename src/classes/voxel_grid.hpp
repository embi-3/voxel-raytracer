#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "aabb.hpp"
#include "direction.hpp"
#include "voxel.hpp"
#include <cmath>
#include <memory>
#include <iostream>

#include <vector>

namespace geometry {
    class VoxelGrid {
    public:
        virtual ~VoxelGrid() = default;

        virtual const Voxel& get_voxel(Coordinate coords) const = 0;

        virtual void set_voxel(Coordinate coords, Voxel voxel) = 0;

        // ! The orientation is used to distinguish ties for coordinates on the border of voxels.
        Coordinate get_coords(Vec3 pos, Direction orientation) const;

        // ! May return a position outside of the bounding box.
        Vec3 get_pos(Coordinate coords) const {
            return origin + Vec3(coords.x * scale.x, coords.y * scale.y, coords.z * scale.z);
        }

        bool contains(Vec3 pos) const {
            return bounding_box.contains(pos);
        }

        bool contains(Coordinate coords) const {
            return coords.x >= 0 && coords.x < size.x && coords.y >= 0 && coords.y < size.y && coords.z >= 0
                   && coords.z < size.z;
        }

        // Returns the distance between the centres of two voxels in 3D space, including scaling.
        num space_dist(Coordinate coord1, Coordinate coord2) const {
            return sqrt(pow((coord1.x - coord2.x) * scale.x, 2) + pow((coord1.y - coord2.y) * scale.y, 2)
                        + pow((coord1.z - coord2.z) * scale.z, 2));
        }

        // Returns the distance between the centres of two voxels in grid space, ignoring scaling.
        num pos_dist(Coordinate coord1, Coordinate coord2) const {
            return sqrt(pow(coord1.x - coord2.x, 2) + pow(coord1.y - coord2.y, 2) + pow(coord1.z - coord2.z, 2));
        }

        // Returns the Manhattan (taxicab) distance between two voxels.
        num man_dist(Coordinate coord1, Coordinate coord2) const {
            return abs(coord1.x - coord2.x) + abs(coord1.y - coord2.y) + abs(coord1.z - coord2.z);
        }

        void create_cube(Coordinate coords) {
            set_voxel(coords, Voxel());
        }

        void create_cube(Coordinate min, Coordinate max);

        void create_sphere(Coordinate centre, num radius);

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

        explicit ArrayVoxelGrid(std::size_t world_size) {
            size = Coordinate(static_cast<int>(world_size));
            initialise();
        }

        explicit ArrayVoxelGrid(std::size_t world_size, Vec3 centre) {
            size = Coordinate(static_cast<int>(world_size));
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        explicit ArrayVoxelGrid(std::size_t x, std::size_t y, std::size_t z) {
            size = Coordinate(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
            initialise();
        }

        explicit ArrayVoxelGrid(std::size_t x, std::size_t y, std::size_t z, Vec3 centre) {
            size = Coordinate(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
            origin =
                centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);
            initialise();
        }

        // TODO: Check if this returns shallow or deep copy of the Voxel.
        const Voxel& get_voxel(Coordinate coords) const override {
            size_t index = flatten(coords);
            // I think at has bounds checking
            if (index >= static_cast<size_t>(size.x * size.y * size.z)) {
                throw std::invalid_argument("Coordinates out of bounds.");
            }

            return world.at(index);
        }

        void set_voxel(Coordinate coords, Voxel voxel) override {
            world.at(flatten(coords)) = voxel;
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

        void initialise();
    };

    class SVOVoxelGrid : public VoxelGrid {
    public:
        explicit SVOVoxelGrid(std::size_t world_size, Vec3 centre) {
            max_depth = static_cast<std::size_t>(ceil(std::log2(world_size)));
            size = Coordinate(static_cast<int>(world_size));
            origin = centre - Vec3((size.x * scale.x) / 2 - 0.5, (size.y * scale.y) / 2 - 0.5, (size.z * scale.z) / 2 - 0.5);

            Vec3 min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
            Vec3 max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }

        const Voxel& get_voxel(Coordinate coords) const override;

        void set_voxel(Coordinate coords, Voxel voxel) override;

    private:
        struct Node {
            bool isLeaf = false;
            std::array<std::unique_ptr<Node>, 8> children{};
            Voxel data = Voxel::empty();
        };

        std::size_t max_depth;
        // std::size_t tree_size = 0;
        std::unique_ptr<Node> root = std::make_unique<Node>();
    };
} // namespace geometry

#endif // VOXELGRID_H