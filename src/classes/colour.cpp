#include "colour.hpp"

namespace texture
{
    Colour normal_blend(const Colour& u, const Colour& v) noexcept {
        return (u * u.a) + ((1 - u.a) * (v * v.a));
    }
} // namespace texture
