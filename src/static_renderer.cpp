#include "static_renderer.hpp"
#include "classes/camera.hpp"
#include "classes/colour.hpp"
#include "classes/ray.hpp"
#include "classes/scene.hpp"
#include "classes/shader.hpp"
#include "classes/vec3.hpp"
#include "voxparser.hpp"

#include "stb_image_write.h"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using Pixel = texture::Colour;
using namespace texture;
using namespace geometry;
using namespace renderer;
using namespace std::chrono;

bool debug = false;
bool svo = false;

// Outputs PPM ASCII and PNG
void create_png(std::size_t width, std::size_t height, const std::vector<Pixel>& pixels) {
    if (pixels.size() != width * height)
        throw std::invalid_argument("Error: Number of pixel entries does not match the expected size.");

    // we are using RGBA
    constexpr auto channels = 4;

    const auto timestamp = system_clock::now().time_since_epoch();
    const auto filename = std::string("output_") + std::to_string(duration_cast<seconds>(timestamp).count());

    auto image = std::vector<uint8_t>{};

    for (const auto& pixel : pixels) {
        image.push_back(pixel.r_int());
        image.push_back(pixel.g_int());
        image.push_back(pixel.b_int());
        // TODO: Check if this is actually doing what it's supposed to.
        image.push_back(static_cast<uint8_t>(255.999 * pixel.a));
    }

    stbi_write_png((filename + ".png").c_str(),
                   static_cast<int>(width),
                   static_cast<int>(height),
                   channels,
                   image.data(),
                   static_cast<int>(channels * width));
}

Scene two_spheres() {
    Scene scene = Scene();

    // ! INFO
    std::cout << "> Creating grid...\n";
    //auto grid = std::make_unique<SVOVoxelGrid>(32, Vec3(0, 0, 20));
    auto grid = std::make_unique<ArrayVoxelGrid>(32, Vec3(0, 0, 20));

    // TODO: Test shapes that go outside the boundary of the grid and see what happens.
    std::cout << "> Creating shapes...\n";
    grid->create_sphere(Coordinate(25, 27, 20), 4);
    grid->create_sphere(Coordinate(10, 5, 10), 10);

    scene.push_back(std::move(grid));

    return scene;
}

Scene random_spheres() {
    int world_size = 200;
    int num_spheres = 300;
    Scene scene = Scene();

    auto grid = std::make_unique<ArrayVoxelGrid>(world_size, Vec3(0, 0, world_size / 2));
    int x;
    int y;
    int z;
    int r;
    for (int i = 0; i < num_spheres; i++) {
        x = rand() % (9 * world_size / 10);
        y = rand() % (9 * world_size / 10);
        z = rand() % (9 * world_size / 10);
        r = rand() % (world_size / 10);
        // Make sure there's no spheres overlapping with the camera
        if (grid->space_dist(Coordinate(x, y, z), grid->get_coords(Vec3(), Direction())) > r) {
            grid->create_sphere(Coordinate(x, y, z), r);
        }
    }
    scene.push_back(std::move(grid));

    return scene;
}


Scene hi_res_spheres() {
    Scene scene = Scene();

    // ! INFO
    std::cout << "> Creating grid...\n";
    std::unique_ptr<VoxelGrid> grid;
    if (svo) {
        grid = std::make_unique<SVOVoxelGrid>(320, Vec3(0, 0, 200));
    } else {
        grid = std::make_unique<ArrayVoxelGrid>(320, Vec3(0, 0, 200));
    }

    // TODO: Test shapes that go outside the boundary of the grid and see what happens.
    std::cout << "> Creating shapes...\n";
    grid->create_sphere(Coordinate(250, 270, 200), 40);
    grid->create_sphere(Coordinate(100, 50, 100), 100);

    scene.push_back(std::move(grid));

    return scene;
}

Scene random_cubes() {
    int world_size = 400;
    int num_cubes = 100;

    Scene scene = Scene();

    std::unique_ptr<VoxelGrid> grid;
    if (svo) {
        grid = std::make_unique<SVOVoxelGrid>(world_size, Vec3(0, 0, 0));
    } else {
        grid = std::make_unique<ArrayVoxelGrid>(world_size, Vec3(0, 0, 0));
    }
    int x;
    int y;
    int z;
    for (int i = 0; i < num_cubes; i++) {
        x = rand() % world_size;
        y = rand() % world_size;
        z = rand() % world_size;
        grid->create_cube(Coordinate(x, y, z), Coordinate(x + 3, y + 3, z + 3));
    }
    scene.push_back(std::move(grid));

    return scene;
}

Scene random_many_cubes() {
    int world_size = 500;
    int num_cubes = 50000;
    Scene scene = Scene();

    std::unique_ptr<VoxelGrid> grid; 

    if (svo) {
        grid = std::make_unique<SVOVoxelGrid>(world_size, world_size, world_size / 4, Vec3(0, 0, world_size / 8));
    } else {
        grid = std::make_unique<ArrayVoxelGrid>(world_size, world_size, world_size / 4, Vec3(0, 0, world_size / 8));
    }
    
    int x;
    int y;
    int z;
    for (int i = 0; i < num_cubes; i++) {
        x = rand() % world_size;
        y = rand() % world_size;
        z = rand() % world_size / 4;
        grid->create_cube(Coordinate(x, y, z));
    }
    scene.push_back(std::move(grid));

    return scene;
}

int main(int argc, char* argv[]) {
    int image_width = 1280;
    int image_height = 720;
    std::string debug_path = "output.txt";
    std::string model_path = "../src/voxmodels/monument/monu1.vox";
    std::string preset = "default";
    bool from_vox = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--width" && i + 1 < argc) {
            image_width = std::atoi(argv[++i]);
        }
        else if (arg == "--height" && i + 1 < argc) {
            image_height = std::atoi(argv[++i]);
        }
        else if (arg == "--debug-path" && i + 1 < argc) {
            debug = true;
        }
        else if (arg == "--debug") {
            debug_path = argv[++i];
        }
        else if (arg == "--model-path" && i + 1 < argc) {
            model_path = argv[++i];
            from_vox = true;
        }
        else if (arg == "--preset" && i + 1 < argc) {
            preset = argv[++i];
        }
        else if (arg == "--svo") {
            svo = true;
        }
        else {
            auto stream = StringStream{};
            stream << "[!] Unknown argument: " << arg << "\n";
            throw std::invalid_argument(stream.str());
        }
    }

    if (debug) {
        std::cout << "Output is being written to a file!\n";
        freopen(debug_path.c_str(), "a", stdout);
        freopen(debug_path.c_str(), "a", stderr);
    }

    // ! INFO
    std::cout << "===========================================\n\n"
              << "> Starting...\n";
    const auto start = high_resolution_clock::now();

    // ! INFO
    std::cout << "> Initialising camera...\n";
    Camera camera = Camera(image_width, image_height);

    // Create the scene
    // ! INFO
    std::cout << "> Creating scene...\n";
    Scene scene = Scene();
    if (from_vox) {
        scene = voxelise(model_path);
    } else if (preset == "default" || preset == "two_spheres") {
        scene = two_spheres();
    } else if (preset == "hi_res_spheres") {
        scene = hi_res_spheres();
    } else if (preset == "cubes" || preset == "rand_cubes") {
        scene = random_cubes();
    } else if (preset == "many_cubes" || preset == "rand_many_cubes") {
        scene = random_many_cubes();
    } else if (preset == "spheres" || preset == "rand_spheres") {   
        scene = random_spheres();
    } else {
        throw new std::invalid_argument("Invalid preset option! Please choose from: [two_spheres, hi_res_spheres, rand_cubes, many_cubes, rand_spheres]\n");
    }

    // ! INFO
    std::cout << " > " << scene.mem_usage() << " bytes required to store the scene.\n";

    // ! INFO
    std::cout << " > " << scene.mem_usage() << " bytes required to store the scene.\n";

    // Create shaders
    // ! INFO
    std::cout << "> Creating shaders...\n";
    GradientShader gradient_shader = GradientShader();
    WhiteShader white_shader = WhiteShader();
    RedShader red_shader = RedShader();
    DistanceShader distance_shader = DistanceShader();
    OrientationShader orientation_shader = OrientationShader();
    RayShader ray_shader = RayShader();
    FlatShader flat_shader = FlatShader();

    // Render the scene
    // ! INFO
    std::cout << "> Rendering scene...\n";
    auto pixels = camera.render(scene, orientation_shader);
    create_png(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);

    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "> Time taken: " << duration << " ms\n\n";
}