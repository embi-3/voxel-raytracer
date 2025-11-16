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
    std::cout << "> Starting...\n";
    const auto start = high_resolution_clock::now();

    int image_width = 400;
    int image_height = 200;

    
    // ! DEBUG
    std::cout << "> Initialising camera...\n";
    Camera camera = Camera(image_width, image_height);

    // Create the scene
    // ! DEBUG
    std::cout << "> Creating scene...\n";
    Scene scene = Scene();

    // ! DEBUG
    std::cout << "> Creating grid...\n";
    VoxelGrid grid = VoxelGrid(10, Vec3(-5, -5, 5));
    std::cerr << "min: " << grid.bounding_box.min << ", max: " << grid.bounding_box.max << "\n";
    // grid.at(5, 5, 5) = Voxel();
    
    // TODO: Test shapes that go outside the boundary of the grid and see what happens.
    std::cout << "> Creating shapes...\n";
    grid.create_sphere(Coordinate(4, 6, 3), 5);
    // grid.create_cube(Coordinate(5, 5, 5), 1);

    scene.push_back(grid);

    // Render the scene
    // ! DEBUG
    std::cout << "> Rendering scene...\n";
    GradientShader gradient_shader = GradientShader();
    WhiteShader white_shader = WhiteShader();
    auto pixels = camera.render(scene, white_shader);
    create_png(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);

    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "> Time taken: " << duration << " ms\n";
}