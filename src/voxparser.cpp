

/*
Parser for the .vox file format.
.vox spec: 
https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*/

#include "voxparser.hpp"
#include "classes/scene.hpp"
#include "classes/colour.hpp"
#include "classes/voxel.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

using namespace geometry;

// Helpers to read int and string ////////////////////////////////////
int readInt(std::ifstream &file) {
    int val;
    file.read(reinterpret_cast<char*>(&val), 4);
    return val;
}

std::string readString(std::ifstream &file, int n) {
    std::string str(static_cast<size_t>(n), '\0');
    file.read(&str[0], n);
    return str;
}
/////////////////////////////////////////////////////////////////////


VoxData renderVoxels(const std::string &filePath) {
    VoxData data;   
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << "\n";
        return data; 
    }

    // read magic 4 bytes and version number 4 bytes
    std::string magic = readString(inputFile, 4);
    if (magic != "VOX ") {
        std::cerr << "Error: Not a .vox file (Wrong magic number)." << std::endl;
    }
    int version = readInt(inputFile);
    (void)version;  // ignore unused variable

    // read MAIN chunk. 
    std::string mainId = readString(inputFile, 4);
    int mainContentSize = readInt(inputFile);   // should be 0
    (void)mainContentSize;  // ignore unused variable
    int mainChildrenSize = readInt(inputFile);  // should be all the other chunks
    auto endOfFile = inputFile.tellg() + static_cast<std::streamoff>(mainChildrenSize);
    
    Model currentModel;
    while (inputFile.tellg() < endOfFile) {

        // read chunk header
        std::string chunkId = readString(inputFile, 4);
        int contentSize = readInt(inputFile);
        int childrenSize = readInt(inputFile);

        if (chunkId == "SIZE") {
            // 5. Read 'SIZE' chunk content
            currentModel.size_x = static_cast<uint32_t>(readInt(inputFile));
            currentModel.size_y = static_cast<uint32_t>(readInt(inputFile));
            currentModel.size_z = static_cast<uint32_t>(readInt(inputFile));
        }
        else if (chunkId == "XYZI") {   // paired with SIZE chunk.
            
            int numVoxels = readInt(inputFile);   
            currentModel.voxels.resize(static_cast<size_t>(numVoxels));

            // load all the voxel info into our model
            inputFile.read(reinterpret_cast<char*>(currentModel.voxels.data()), numVoxels * 4);
            
            // model is complete, save it to data 
            data.model = currentModel;  
        }
        else if (chunkId == "RGBA") {
            inputFile.read(reinterpret_cast<char*>(data.palette), 256 * 4);
            data.palette_set = true;
        }
        else {  
            // disregarding chunkId = "PACK" for now - not working with animation
            // Unknown chunk, skip it
            inputFile.seekg(contentSize + childrenSize, std::ios::cur);
        }
    }
    inputFile.close();
    return data;
}



Scene voxelise(const std::string &filePath) {

    VoxData data = renderVoxels(filePath);   

    Scene scene = Scene();

    auto grid = std::make_unique<SVOVoxelGrid>(
        data.model.size_x + 2,
        data.model.size_z + 2,
        data.model.size_y + 2,
        // TODO: Decide how far to place the camera away from the model.
        // This won't matter after camera movement is implemented.
        Vec3(0, 0, data.model.size_y + 1) 
    );

    for (size_t i = 0; i < data.model.voxels.size(); i++) {
        int currColourIndex = data.model.voxels[i].colourIndex;
        Colour currVoxelColour = Colour(
            data.palette[currColourIndex].r,
            data.palette[currColourIndex].g,
            data.palette[currColourIndex].b, 1
        );

        Voxel currVoxel = Voxel(currVoxelColour);
        Coordinate currVoxelCoord = Coordinate(
            data.model.voxels[i].x + 1, 
            data.model.voxels[i].z + 1,
            data.model.voxels[i].y + 1      
            // we use y = up instead of z = up
        );

        grid->set_voxel(currVoxelCoord, currVoxel);
    }
    //grid->compress();
    scene.push_back(std::move(grid));
    return scene;
}
