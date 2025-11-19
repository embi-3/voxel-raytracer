

/*
Parser for the .vox file format.
.vox spec: 
https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*/

/*


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

// TODO: REFACTOR THIS INTO OUR DEFINED CLASSES  ////////////////////////
// TODO: REFACTOR THIS INTO OUR DEFINED CLASSES  ////////////////////////
// TODO: REFACTOR THIS INTO OUR DEFINED CLASSES  ////////////////////////
// TODO: REFACTOR THIS INTO OUR DEFINED CLASSES  ////////////////////////


// 'RGBA' chunk: 4 bytes (R, G, B, A)
struct Color {
    uint8_t r, g, b, a;
};

// 'XYZI' chunk: 4 bytes (x, y, z, colorIndex)
struct Voxel {
    uint8_t x, y, z, colorIndex;
};

// A struct to hold a single model's data
struct Model {
    uint32_t size_x;
    uint32_t size_y;
    uint32_t size_z;
    std::vector<Voxel> voxels; // A dynamic array of voxels
};

// A struct to hold the entire file's contents
struct VoxData {
    std::vector<Model> models;
    Color palette[256];
    bool palette_set = false;
};
/////////////////////////////////////////////////////////////////////////


int readInt(std::ifstream &file) {
    int val;
    file.read(reinterpret_cast<char*>(&val), 4);
    return val;
}

std::string readString(std::ifstream &file, int n) {
    std::string str(n, '\0');
    file.read(&str[0], n);
    return str;
}

int main() {

    VoxData data;   // stores whole file's contents 

    // open the file in binary 
    std::ifstream inputFile("/src/voxmodels/scan/teapot.vox", std::ios::binary);

    // read magic 4 bytes and version number 4 bytes
    std::string magic = readString(inputFile, 4);
    if (magic != "VOX ") {
        std::cerr << "Error: Not a .vox file (Wrong magic number)." << std::endl;
    }
    int version = readInt(inputFile);

    // read MAIN chunk. 
    std::string mainId = readString(inputFile, 4);
    int mainContentSize = readInt(inputFile);   // should be 0
    int mainChildrenSize = readInt(inputFile);  // should be all the other chunks
    int endOfFile = inputFile.tellg() + mainChildrenSize;
    
    Model currentModel;
    while (inputFile.tellg() < endOfFile) {

        // read chunk header
        std::string chunkId = readString(inputFile, 4);
        int contentSize = readInt(inputFile);
        int childrenSize = readInt(inputFile);

        if (chunkId == "SIZE") {
            // 5. Read 'SIZE' chunk content
            currentModel.size_x = readInt(inputFile);
            currentModel.size_y = readInt(inputFile);
            currentModel.size_z = readInt(inputFile);
        }
        else if (chunkId == "XYZI") {   // paired with SIZE chunk.
            int numVoxels = readInt(inputFile);
            currentModel.voxels.resize(numVoxels);

            // load all the voxel info into our model
            inputFile.read(reinterpret_cast<char*>(currentModel.voxels.data()), numVoxels * 4);
            
            // model is complete, add it to model list
            data.models.push_back(currentModel);
            
            // resets model (though i don't think there will be more than 1 model)
            // useful for PACK if we ever decide to do animation 
            currentModel = Model();     
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
}

*/

