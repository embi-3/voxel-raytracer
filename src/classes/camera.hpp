#ifndef CAMERA_H
#define CAMERA_H

#include "../common.hpp"

#include "colour.hpp"
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
        explicit Camera(int image_width = 1920, int image_height = 1080, num focal_length = 1.0, num viewport_height = 2.0)
        : image_width(image_width)
        , image_height(image_height)
        , aspect_ratio(image_width / image_height)
        , focal_length(focal_length)
        , viewport_height(viewport_height)
        , viewport_width(viewport_height * (static_cast<double>(image_width) / image_height)) {
            // calculate horizontal and vertical viewport vectors
            const auto viewport_x = Vec3(viewport_width, 0, 0);
            const auto viewport_y = Vec3(0, -viewport_height, 0);

            // delta vectors from pixel to pixel
            delta_x = viewport_x / image_width;
            delta_y = viewport_y / image_height;

            // vector for upper left pixel
            const auto viewport_upper_left = camera_center + Vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;
            upper_left_pixel = viewport_upper_left + 0.5 * (delta_x + delta_y);
        }

        std::vector<Pixel> render(Scene& scene, Shader& shader) const;

        std::vector<Pixel> render(VoxelGrid& grid, Shader& shader);

    private:
        int image_width;
        int image_height;
        [[maybe_unused]] num aspect_ratio;
        [[maybe_unused]] num focal_length;
        Vec3 camera_center = Vec3();
        [[maybe_unused]] num viewport_height;
        num viewport_width;
        Vec3 upper_left_pixel;
        Vec3 delta_x;
        Vec3 delta_y;
    };
} // namespace renderer

#endif // CAMERA_H