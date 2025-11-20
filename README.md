## Overview
This project is a voxel-based ray tracer which takes ideas from several papers.

Ray traversal was based on the Fast Voxel Traversal Algorithm by Amanatides and Woo: http://www.cse.yorku.ca/~amana/research/grid.pdf

Voxel grid data was stored naively in a linear array and more efficiently using a Sparse Voxel Octree inspired by Laine and Karras: https://research.nvidia.com/sites/default/files/pubs/2010-02_Efficient-Sparse-Voxel/laine2010tr1_paper.pdf

The ideas in these papers have been extended to further improve the performance and capabilities of our renderer. Our renderer correctly identifies and traverses diagonals and corners, an edge case that was noticeably absent from the original paper. Furthermore, we have converted the traversal algorithm to work in 3D space, while the proposal was originally designed for 2D rendering. We have also drawn inspiration from ideas in the paper to accelerate ray traversal using the Sparse Voxel Octree. The sparsity of the graph allows us to skip large regions of empty space when traversing through the octree. Finally, our renderer is able to render transparent voxel data, allowing for a wider range of models, including water, smoke and fog.

## How to Use
Firstly, clone the repository:
```
https://github.com/embi-3/voxel-raytracer.git
```

Navigate to the root directory of the repository and build the project:
```
cmake -B build
cd build
make
```

To run the program, run the following while in the `build` directory:
```
./static_raytracer
```

### Flags
Several flags can be used to modify the behaviour of the program:
- `--width <width>` and `--height <height>` can be used to specify the width and height of the output image. By default, the program will choose `1920` and `1080` respectively.
- `--debug` prints diagnostic and debug information for the ray tracer. By default, this will print to `std::cerr`. **BE WARNED, THIS IS A LOT OF OUTPUT! IT IS RECOMMENDED TO WRITE THIS TO A FILE INSTEAD!**
- `--debug-path <path>` specifies a path to write the debug output to. Does nothing if `--debug` is not enabled.
- `--model-path <path>` specifies a path to a `.vox` file to render. `<path>` can be an absolute path, or a relative path from the `src` directory.


### Models
The program comes with several `.vox` files included already, which can be found in `src/voxmodels`. These models were sourced from: https://github.com/ephtracy/voxel-model/tree/master/vox. The program has a built in parser to convert `.vox` files into a C++ object. See [https://github.com/embi-3/voxel-raytracer?tab=readme-ov-file#flags](Flags) to specify a path to a `.vox` file.
