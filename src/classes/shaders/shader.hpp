#ifndef SHADER_H
#define SHADER_H

#include "../../common.hpp"
#include "../colour.hpp"
#include "../intersection_list.hpp"

#include <vector>

namespace shader {
    using namespace texture;
    using namespace geometry;

    class Shader {
    public:
        virtual Colour fragment(IntersectionList list) = 0;

        virtual ~Shader() = default;
    };

    // ? Is it bad practice to put these classes here instead of in their own files?
    class WhiteShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                // ! DEBUG
                // ! temporarily make it red for clarity
                return Colour::red();
                // return Colour::white();
            }
            return Colour();
        }
    };

    // ? Is it bad practice to put these classes here instead of in their own files?
    class GradientShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            Vec3 unit_direction = list.ray_dir.normalise();
            auto a = 0.5 * (unit_direction.y + 1.0);
            return interpolate(Colour(124, 179, 255), Colour::white(), a);
        }
    };
} // namespace shader

#endif // SHADER_H
