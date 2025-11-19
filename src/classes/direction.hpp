#ifndef DIRECTION_H
#define DIRECTION_H

#include "../common.hpp"
#include "vec3.hpp"
#include <string>

namespace geometry {
    static constexpr uint8_t NONE = 0;
    static constexpr uint8_t X_POS = 1;
    static constexpr uint8_t X_NEG = 2;
    static constexpr uint8_t Y_POS = 4;
    static constexpr uint8_t Y_NEG = 8;
    static constexpr uint8_t Z_POS = 16;
    static constexpr uint8_t Z_NEG = 32;

    class Direction {
    public:
        constexpr Direction() = default;

        constexpr Direction(uint8_t dir) noexcept
        : dir(dir) {}

        constexpr Direction(Vec3 direction) noexcept {
            uint8_t x_dir = Direction::from_x_orient(direction.x).dir;
            uint8_t y_dir = Direction::from_y_orient(direction.y).dir;
            uint8_t z_dir = Direction::from_z_orient(direction.z).dir;

            dir = x_dir | y_dir | z_dir;
        }

        bool is_none() const noexcept {
            return dir == 0;
        }

        bool is_xpos() const noexcept {
            return dir & X_POS;
        }

        bool is_xneg() const noexcept {
            return dir & X_NEG;
        }

        bool is_ypos() const noexcept {
            return dir & Y_POS;
        }

        bool is_yneg() const noexcept {
            return dir & Y_NEG;
        }

        bool is_zpos() const noexcept {
            return dir & Z_POS;
        }

        bool is_zneg() const noexcept {
            return dir & Z_NEG;
        }

        bool is_x() const noexcept {
            return is_xpos() || is_xneg();
        }

        bool is_y() const noexcept {
            return is_ypos() || is_yneg();
        }

        bool is_z() const noexcept {
            return is_zpos() || is_zneg();
        }

        constexpr static Direction from_x_orient(num orientation) noexcept {
            if (orientation > 0) {
                return Direction(X_POS);
            }
            else if (orientation < 0) {
                return Direction(X_NEG);
            }
            else {
                return Direction(NONE);
            }
        }

        constexpr static Direction from_y_orient(num orientation) noexcept {
            if (orientation > 0) {
                return Direction(Y_POS);
            }
            else if (orientation < 0) {
                return Direction(Y_NEG);
            }
            else {
                return Direction(NONE);
            }
        }

        constexpr static Direction from_z_orient(num orientation) noexcept {
            if (orientation > 0) {
                return Direction(Z_POS);
            }
            else if (orientation < 0) {
                return Direction(Z_NEG);
            }
            else {
                return Direction(NONE);
            }
        }

        Direction x() const noexcept {
            return Direction(dir & (X_POS | X_NEG));
        }

        Direction y() const noexcept {
            return Direction(dir & (Y_POS | Y_NEG));
        }

        Direction z() const noexcept {
            return Direction(dir & (Z_POS | Z_NEG));
        }

        Coordinate sign() const noexcept {
            int x = (is_xpos() ? 1 : (is_xneg() ? -1 : 0));
            int y = (is_ypos() ? 1 : (is_yneg() ? -1 : 0));
            int z = (is_zpos() ? 1 : (is_zneg() ? -1 : 0));
            return Coordinate(x, y, z);
        }

        Direction& operator+=(const Direction& rhs) noexcept {
            dir |= rhs.dir;
            return *this;
        }

        friend Direction operator+(const Direction& u, const Direction& v) noexcept;
        friend std::ostream& operator<<(std::ostream& out, const Direction& dir);

    private:
        uint8_t dir = 0;
    };

    inline Direction operator+(const Direction& u, const Direction& v) noexcept {
        return Direction(u.dir | v.dir);
    }

    inline std::ostream& operator<<(std::ostream& out, const Direction& dir) {
        std::string str = "";
        if (dir.is_none()) {
            str.append("NONE ");
        }
        if (dir.is_xpos()) {
            str.append("X_POS ");
        }
        if (dir.is_xneg()) {
            str.append("X_NEG ");
        }
        if (dir.is_ypos()) {
            str.append("Y_POS ");
        }
        if (dir.is_yneg()) {
            str.append("Y_NEG ");
        }
        if (dir.is_zpos()) {
            str.append("Z_POS ");
        }
        if (dir.is_zneg()) {
            str.append("Z_NEG ");
        }

        return out << str;
    }
} // namespace geometry

#endif // DIRECTION_H