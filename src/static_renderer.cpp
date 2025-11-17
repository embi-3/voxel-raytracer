#include "static_renderer.hpp"
#include "classes/camera.hpp"
#include "classes/colour.hpp"
#include "classes/ray.hpp"
#include "classes/scene.hpp"
#include "classes/shaders/shader.hpp"
#include "classes/vec3.hpp"

#include "stb_image_write.h"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using Pixel = texture::Colour;
using namespace texture;
using namespace geometry;
using namespace renderer;
using namespace std::chrono;

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
        image.push_back(static_cast<uint8_t>(255.999 * pixel.a));
    }

    stbi_write_png((filename + ".png").c_str(),
                   static_cast<int>(width),
                   static_cast<int>(height),
                   channels,
                   image.data(),
                   static_cast<int>(channels * width));
}

int main() {
    // ! DEBUG
    std::cerr << "Output is being written to a file!\n";
    freopen("output.txt", "a", stdout);
    freopen("output.txt", "a", stderr);

    // freopen("/dev/null", "a", stdout);
    // freopen("/dev/null", "a", stderr);

    // ! DEBUG
    std::cout << "===========================================\n\n"
              << "> Starting...\n";
    const auto start = high_resolution_clock::now();

    // int image_width = 400;
    // int image_height = 200;
    
    // ! Try odd dimensions to test edge cases with 0.
    int image_width = 401;
    int image_height = 201;

    // ! DEBUG
    std::cout << "> Initialising camera...\n";
    Camera camera = Camera(image_width, image_height);

    // Create the scene
    // ! DEBUG
    std::cout << "> Creating scene...\n";
    Scene scene = Scene();

    // ! DEBUG
    std::cout << "> Creating grid...\n";
    VoxelGrid grid = VoxelGrid(32, Vec3(0, 0, 20));

    // ! DEBUG
    std::cerr << "min: " << grid.bounding_box.min << ", max: " << grid.bounding_box.max << "\n";
    // grid.get_voxel(5, 5, 5) = Voxel();
    // grid.set_voxel(5, 5, 5, Voxel());

    // std::cout << grid.get_voxel(5, 5, 5).value().is_opaque() << "\n";

    // TODO: Test shapes that go outside the boundary of the grid and see what happens.
    std::cerr << "> Creating shapes...\n" << std::flush;
    grid.create_sphere(Coordinate(16, 16, 16), 10);
    // grid.create_sphere(Coordinate(10, 5, 5), 4);
    // grid.create_cube(Coordinate(6, 6, 6), Coordinate(26, 26, 26));
    // grid.create_cube(Coordinate(0, 0, 1));
    // grid.create_cube(Coordinate(15, 15, 0));
    // grid.create_cube(Coordinate(5, 5, 10));

    scene.push_back(grid);

    // Render the scene
    // ! DEBUG
    std::cout << "> Rendering scene...\n";
    GradientShader gradient_shader = GradientShader();
    WhiteShader white_shader = WhiteShader();
    RedShader red_shader = RedShader();
    DistanceShader distance_shader = DistanceShader();
    OrientationShader normal_shader = OrientationShader();
    RayShader ray_shader = RayShader();
    auto pixels = camera.render(scene, ray_shader);
    create_png(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);

    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "> Time taken: " << duration << " ms\n\n";
}