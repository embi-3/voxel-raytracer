#ifndef GRADIENT_SHADER_H
#define GRADIENT_SHADER_H

#include "../../common.hpp"
#include "../colour.hpp"
#include "../intersection_list.hpp"
#include "shader.hpp"

#include <vector>

namespace shader {
    using namespace texture;
    using namespace geometry;

    class GradientShader: public Shader {
    public:
        Colour fragment(IntersectionList list) {
            Vec3 unit_direction = list.ray_dir.normalise();
            auto a = 0.5 * (unit_direction.y + 1.0);
            return interpolate(Colour(124, 179, 255), Colour::white(), a);
        }
    };
} // namespace shader

#endif // GRADIENT_SHADER_H
