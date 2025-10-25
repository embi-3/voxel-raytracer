#include "static_renderer.hpp"
#include "classes/colour.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using Pixel = texture::Colour;

// Currently uses PPM ASCII
void createPPM(std::size_t width, std::size_t height, const std::vector<Pixel>& pixels) {
    if (pixels.size() != width * height)
        throw std::invalid_argument("Error: Number of pixel entries does not match the expected size.");

    auto file = std::ofstream{"output.ppm"};

    // PPM header
    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& pixel : pixels)
        file << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " " << static_cast<int>(pixel.b) << " ";

    file.close();
}

int main() {
    constexpr auto height = 256;
    constexpr auto width = 256;

    auto pixels = std::vector<Pixel>{};

    // Just copied the image provided by the guide below for testing purposes (Section 2.1)
    // https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            const auto r = double(i) / (width - 1);
            const auto g = double(j) / (height - 1);
            const auto b = 0.0;

            const auto ir = uint8_t(255.999 * r);
            const auto ig = uint8_t(255.999 * g);
            const auto ib = uint8_t(255.999 * b);
            
            const auto pixel = Pixel{ir, ig, ib};
            pixels.push_back(pixel);
        }
    }

    createPPM(width, height, pixels);

    return 0;
}
