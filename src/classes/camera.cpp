#include "ray.hpp"

#include "camera.hpp"

namespace renderer {
    std::vector<Pixel> Camera::render(Scene& scene, Shader& shader) const {
        auto progress = 0;
        auto current_pixels = 0;
        const auto total_pixels = image_height * image_width;
        const auto chunk = total_pixels / 10;
        auto pixels = std::vector<Pixel>{}; // array of pixels

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                current_pixels = j * image_width + i;
                if (current_pixels / chunk >= progress) {
                    // ! INFO
                    std::cout << "- " << (progress++ * 10) << "% done"
                              << " \n";
                }
                const auto pixel_center = upper_left_pixel + (i * delta_x) + (j * delta_y);
                const auto ray_direction = pixel_center - camera_center;
                auto r = Ray{camera_center, ray_direction};

                // ! DEBUG
                if (debug) {
                    std::cerr << "Pixel: (" << i << ", " << j << ")\n";
                }

                const auto intersections = r.traverse(scene);
                const auto pixel_colour = shader.fragment(intersections);
                pixels.push_back(pixel_colour);
            }
        }
        // ! INFO
        std::cout << "- 100% done \n";

        return pixels;
    }

    std::vector<Pixel> Camera::render(VoxelGrid& grid, Shader& shader) {
        auto pixels = std::vector<Pixel>{}; // array of pixels
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = upper_left_pixel + (i * delta_x) + (j * delta_y);
                auto ray_direction = pixel_center - camera_center;
                auto r = Ray{camera_center, ray_direction};

                auto intersections = r.traverse(grid);
                auto pixel_colour = shader.fragment(intersections);
                pixels.push_back(pixel_colour);
            }
        }
        return pixels;
    }
} // namespace renderer
