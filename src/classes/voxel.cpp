#include "voxel.hpp"

namespace geometry
{
    bool Voxel::is_invalid() const {
        return (*this) == geometry::INVALID_VOXEL;
    }
} // namespace geometry