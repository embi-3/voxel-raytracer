#include "vec3.hpp"

namespace geometry {    
    // ! Double check this does what it should.
    Vec3 Vec3::reflect(const Vec3& n) const {
        const Vec3 v(*this);
        return v - (2 * v.dot(n) * n);
    }

    Vec3 Vec3::into_unit() const {
        return *this / length();
    }  
}
