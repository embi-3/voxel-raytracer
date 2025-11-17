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
                return Colour::white();
            }
            return Colour();
        }
    };

    class RedShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                return Colour::red();
            }
            return Colour();
        }
    };

    class DistanceShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                num distance = (list.items.at(0)).distance;
                // ! DEBUG
                std::cout << "Distance: " << distance << "\n";
                return Colour(distance - 10);
            }
            return Colour();
        }
    };

    class NormalShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                Direction normal = (list.items.at(0)).normal;
                Colour colour = Colour::black();
                if (normal.is_xpos()) {
                    colour += Colour::red();
                }
                if (normal.is_xneg()) {
                    colour += Colour::magenta();
                }
                if (normal.is_ypos()) {
                    colour += Colour::blue();
                }
                if (normal.is_yneg()) {
                    colour += Colour::yellow();
                }
                if (normal.is_zpos()) {
                    colour += Colour::green();
                }
                if (normal.is_zneg()) {
                    colour += Colour::cyan();
                }

                return colour;
            }
            return Colour();
        }
    };

    class RayShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                Vec3 dir = list.ray_dir.normalise();
                Colour colour = Colour(255 * dir.x, 255 * dir.y, 255 * dir.z);

                return colour;
            }
            return Colour();
        }
    };

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
