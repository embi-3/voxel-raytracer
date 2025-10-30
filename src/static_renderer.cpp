#include "static_renderer.hpp"
#include "classes/colour.hpp"
#include "classes/ray.hpp"
#include "classes/vec3.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"

#pragma clang diagnostic pop

#include <cstdint>
#include <fstream>
#include <vector>

using Pixel = texture::Colour;

// Outputs PPM ASCII and PNG
void create_image(std::size_t width, std::size_t height, const std::vector<Pixel>& pixels) {
    if (pixels.size() != width * height)
        throw std::invalid_argument("Error: Number of pixel entries does not match the expected size.");

    // we are using RGBA
    constexpr auto channels = 4;

    auto file = std::ofstream("output.ppm");

    // PPM header
    file << "P3\n" << width << " " << height << "\n255\n";

    auto image = std::vector<uint8_t>{};

    // scale from 0.0-1.0 to 0-255
    for (const auto& pixel : pixels) {
        auto r_int = static_cast<uint8_t>(255.999 * pixel.r);
        auto g_int = static_cast<uint8_t>(255.999 * pixel.g);
        auto b_int = static_cast<uint8_t>(255.999 * pixel.b);
        auto a_int = static_cast<uint8_t>(255.999 * pixel.a);

        // Write to PPM
        file << static_cast<int>(r_int) << " " << static_cast<int>(g_int) << " " << static_cast<int>(b_int) << " ";

        image.push_back(r_int);
        image.push_back(g_int);
        image.push_back(b_int);
        image.push_back(a_int);
    }

    file.close();

    stbi_write_png("output.png",
                   static_cast<int>(width),
                   static_cast<int>(height),
                   channels,
                   image.data(),
                   static_cast<int>(channels * width));
}

// simple gradient from 4.2 from "Ray Tracing In One Weekend" website
texture::Colour ray_colour(const geometry::Ray& r) {
    geometry::Vec3 unit_direction = (r.dir).normalise();
    auto a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * texture::Colour::white() + a * texture::Colour(124, 179, 255);
}

int main() {
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

            auto pixel_colour = ray_colour(r);
            pixels.push_back(pixel_colour);
        }
    }
    create_image(static_cast<std::size_t>(image_width), static_cast<std::size_t>(image_height), pixels);
}
