#ifndef SHADER_H
#define SHADER_H

#include "../common.hpp"
#include "colour.hpp"
#include "intersection_list.hpp"

#include <vector>

namespace shader {
    using namespace texture;
    using namespace geometry;

    class Shader {
    public:
        virtual Colour fragment(const IntersectionList& list) const = 0;

        virtual ~Shader() = default;
    };

    // ? Is it bad practice to put these classes here instead of in their own files?
    class WhiteShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                return Colour::white();
            }
            return Colour();
        }
    };

    class RedShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                return Colour::red();
            }
            return Colour();
        }
    };

    class DistanceShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                num distance = (list.at(0)).distance;
                // ! DEBUG
                if (debug) {
                    std::cerr << "Distance: " << distance << "\n";
                }
                return Colour((1 / distance) * 255);
            }
            return Colour();
        }
    };

    class OrientationShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                Direction orientation = (list.at(0)).orientation;
                Colour colour = Colour::black();
                if (orientation.is_xpos()) {
                    colour += Colour::red();
                }
                if (orientation.is_xneg()) {
                    colour += Colour::magenta();
                }
                if (orientation.is_ypos()) {
                    colour += Colour::blue();
                }
                if (orientation.is_yneg()) {
                    colour += Colour::yellow();
                }
                if (orientation.is_zpos()) {
                    colour += Colour::green();
                }
                if (orientation.is_zneg()) {
                    colour += Colour::cyan();
                }

                return colour;
            }
            return Colour();
        }
    };

    class RayShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                Vec3 dir = list.ray_direction();
                Colour colour = Colour(122.5) + Colour(122.5 * dir.x, 122.5 * dir.y, 122.5 * dir.z);

                return colour;
            }
            return Colour();
        }
    };

    class GradientShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            Vec3 unit_direction = list.ray_direction();
            auto a = 0.5 * (unit_direction.y + 1.0);
            return interpolate(Colour(124, 179, 255), Colour::white(), a);
        }
    };

    class FlatShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                const auto& hitVoxel = list.items.at(0);
                return hitVoxel.voxel.get_colour();
            }
            return Colour();
        }
    };

    class TransparentShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            size_t index = 0;
            num alpha = 255;
            Colour colour = Colour();
            while (index < list.size() && !equals_zero(alpha)) {
                const auto& hitColour = list.items.at(index).voxel.get_colour();
                const auto hitAlpha = hitColour.a;
                colour.r = colour.r * colour.a + hitColour.r * alpha * hitAlpha;
                colour.g = colour.g * colour.a + hitColour.g * alpha * hitAlpha;
                colour.b = colour.b * colour.a + hitColour.b * alpha * hitAlpha;
                colour.a = colour.a + alpha * hitAlpha;
                alpha = std::max(alpha * (1 - hitAlpha), 0.0);
            }

            return colour;
        }
    };


    class IterationShader : public Shader {
    public:
        Colour fragment(const IntersectionList& list) const override {
            if (!list.empty()) {
                const auto& hitVoxel = list.items.at(0);
                return hitVoxel.voxel.get_colour();
            }
            return Colour();
        }
    };
} // namespace shader

#endif // SHADER_H
