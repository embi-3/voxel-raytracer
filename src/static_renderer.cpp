#include "static_renderer.hpp"
#include "classes/colour.hpp"
#include "classes/ray.hpp"
#include "classes/vec3.hpp"

#include "stb_image_write.h"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using Pixel = texture::Colour;
using namespace std::chrono;

// Outputs PPM ASCII and PNG
void create_image(std::size_t width, std::size_t height, const std::vector<Pixel>& pixels) {
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

// simple gradient from 4.2 from "Ray Tracing In One Weekend" website
Pixel gradient(const geometry::Ray& r) {
    geometry::Vec3 unit_direction = r.dir.normalise();
    auto a = 0.5 * (unit_direction.y + 1.0);
    return texture::interpolate(Pixel(124, 179, 255), Pixel::white(), a);
}

int main() {
    const auto start = high_resolution_clock::now();

    // 16:9 aspect ratio, width of 400
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // finds height from ratio and width (height at least 1)
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = geometry::Point3(0, 0, 0);

    // calculate horizontal and vertical viewport vectors
    auto viewport_u = geometry::Vec3(viewport_width, 0, 0);
    auto viewport_v = geometry::Vec3(0, -viewport_height, 0);

    // delta vectors from pixel to pixel
    auto delta_u = viewport_u / image_width;
    auto delta_v = viewport_v / image_height;

    // vector for upper left pixel
    auto viewport_upper_left = camera_center - geometry::Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto upper_left_pixel = viewport_upper_left + 0.5 * (delta_u + delta_v);

    // render
    auto pixels = std::vector<Pixel>{}; // array of pixels
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = upper_left_pixel + (i * delta_u) + (j * delta_v);
            auto ray_direction = pixel_center - camera_center;
            auto r = geometry::Ray{camera_center, ray_direction};

            auto pixel_colour = gradient(r);
            pixels.push_back(pixel_colour);
        }
    }
    create_image(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);

    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "Time taken: " << duration << " ms\n";
}
