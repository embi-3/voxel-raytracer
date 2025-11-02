#include "colour.hpp"

namespace texture
{
    Colour normal_blend(const Colour& u, const Colour& v) noexcept {
        return (u * u.a) + ((1 - u.a) * (v * v.a)).set_alpha(std::min(u.a + (u.a * v.a), 1.0));
    }
} // namespace texture
