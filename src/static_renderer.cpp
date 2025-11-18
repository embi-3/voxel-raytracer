#include "static_renderer.hpp"
#include "classes/camera.hpp"
#include "classes/colour.hpp"
#include "classes/ray.hpp"
#include "classes/scene.hpp"
#include "classes/shader.hpp"
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
    // ! INFO
    // std::cerr << "Output is being written to a file!\n";
    // freopen("output.txt", "a", stdout);
    // freopen("output.txt", "a", stderr);

    // freopen("/dev/null", "a", stdout);
    // freopen("/dev/null", "a", stderr);

    // ! INFO
    std::cout << "===========================================\n\n"
              << "> Starting...\n";
    const auto start = high_resolution_clock::now();

    int image_width = 1920;
    int image_height = 1080;

    // ! INFO
    std::cout << "> Initialising camera...\n";
    Camera camera = Camera(image_width, image_height);

    // Create the scene
    // ! INFO
    std::cout << "> Creating scene...\n";
    Scene scene = Scene();

    // ! INFO
    std::cout << "> Creating grid...\n";
    auto grid = SVOVoxelGrid(32, Vec3(0, 0, 20));

    // TODO: Test shapes that go outside the boundary of the grid and see what happens.
    std::cout << "> Creating shapes...\n";
    grid.create_sphere(Coordinate(25, 27, 20), 4);
    grid.create_sphere(Coordinate(10, 5, 10), 10);
    // grid.create_cube(Coordinate(6, 6, 6), Coordinate(26, 26, 26));
    // grid.create_cube(Coordinate(0, 0, 1));
    // grid.create_cube(Coordinate(15, 15, 0));
    // grid.create_cube(Coordinate(5, 5, 10));

    scene.push_back(grid);

    // Create shaders
    // ! INFO
    std::cout << "> Creating shaders...\n";
    GradientShader gradient_shader = GradientShader();
    WhiteShader white_shader = WhiteShader();
    RedShader red_shader = RedShader();
    DistanceShader distance_shader = DistanceShader();
    OrientationShader orientation_shader = OrientationShader();
    RayShader ray_shader = RayShader();

    // Render the scene
    // ! INFO
    std::cout << "> Rendering scene...\n";
    auto pixels = camera.render(scene, orientation_shader);
    create_png(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);

    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "> Time taken: " << duration << " ms\n\n";
}