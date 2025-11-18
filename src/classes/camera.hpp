#ifndef CAMERA_H
#define CAMERA_H

#include "../common.hpp"
#include "colour.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "vec3.hpp"

#include <vector>

namespace renderer {
    using Pixel = texture::Colour;
    using namespace geometry;
    using namespace shader;
    class Camera {
    public:
        int image_width;
        int image_height;
        num aspect_ratio;
        num focal_length;
        Vec3 camera_center = Vec3();
        num viewport_height;
        num viewport_width;
        Vec3 upper_left_pixel;
        Vec3 delta_x;
        Vec3 delta_y;

        explicit Camera(int image_width = 1920, int image_height = 1080, num focal_length = 1.0, num viewport_height = 2.0)
        : image_width(image_width)
        , image_height(image_height)
        , focal_length(focal_length)
        , viewport_height(viewport_height) {
            aspect_ratio = image_width / image_height;

            viewport_width = viewport_height * (double(image_width) / image_height);

            // calculate horizontal and vertical viewport vectors
            auto viewport_x = Vec3(viewport_width, 0, 0);
            auto viewport_y = Vec3(0, -viewport_height, 0);

            // delta vectors from pixel to pixel
            delta_x = viewport_x / image_width;
            delta_y = viewport_y / image_height;

            // vector for upper left pixel
            auto viewport_upper_left = camera_center + Vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;
            upper_left_pixel = viewport_upper_left + 0.5 * (delta_x + delta_y);
        }

        std::vector<Pixel> render(Scene& scene, Shader& shader) {
            int progress = 0;
            int total_pixels = image_height * image_width;
            int current_pixels = 0;
            int chunk = total_pixels / 10;
            auto pixels = std::vector<Pixel>{}; // array of pixels
            for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; i++) {
                    current_pixels = j * image_width + i;
                    if (current_pixels / chunk >= progress) {
                        // ! INFO
                        std::cout << "- " << (progress++ * 10) << "% done"
                                  << " \n";
                    }
                    auto pixel_center = upper_left_pixel + (i * delta_x) + (j * delta_y);
                    auto ray_direction = pixel_center - camera_center;
                    auto r = Ray{camera_center, ray_direction};

                    // ! DEBUG
                    std::cerr << "Pixel: (" << i << ", " << j << ")\n";

                    auto intersections = r.traverse(scene);
                    auto pixel_colour = shader.fragment(intersections);
                    pixels.push_back(pixel_colour);
                }
            }
            // ! INFO
            std::cout << "- 100% done \n";
            
            return pixels;
        }

        std::vector<Pixel> render(VoxelGrid& grid, Shader& shader) {
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
    };
} // namespace renderer

#endif // CAMERA_H