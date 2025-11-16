// TODO: Rename this define!
#ifndef TEMPLATE_SHADER_H
#define TEMPLATE_SHADER_H

#include "../../common.hpp"
#include "../colour.hpp"
#include "../intersection_list.hpp"
#include "shader.hpp"

#include <vector>

namespace shader {
    using namespace texture;
    using namespace geometry;

    // TODO: Name this class!
    class TemplateShader: public Shader {
    public:
        Colour fragment(IntersectionList list) override {
            // TODO: Implement this function!
        }
    };
} // namespace shader

#endif // TEMPLATE_SHADER_H
