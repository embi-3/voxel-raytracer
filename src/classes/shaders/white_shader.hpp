#ifndef WHITE_SHADER_H
#define WHITE_SHADER_H

#include "../../common.hpp"
#include "../colour.hpp"
#include "../intersection_list.hpp"
#include "shader.hpp"

#include <vector>

namespace shader {
    using namespace texture;
    using namespace geometry;

    class WhiteShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            if (!list.empty()) {
                return Colour::white();
            }
            return Colour();
        }
    };
} // namespace shader

#endif // WHITE_SHADER_H
