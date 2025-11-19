
#ifndef VOXPARSER_H
#define VOXPARSER_H

#include "classes/scene.hpp"
#include "classes/colour.hpp"
#include "classes/voxel.hpp"

#include <vector>
#include <string>
#include <cstdint>


struct VoxelColour {
    uint8_t r, g, b, a;
};

struct Vox {
    uint8_t x, y, z, colourIndex;
};

struct Model {
    uint32_t size_x, size_y, size_z;
    std::vector<Vox> voxels;
};

struct VoxData {
    Model model;
    VoxelColour palette[256];
    bool palette_set = false;
};

geometry::Scene voxelise(const std::string &filePath);

#endif