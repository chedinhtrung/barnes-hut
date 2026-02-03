#pragma once
#include <cmath>

/*
Vec3 type for forces, positions and velocities
*/
struct Vec3 {
    double x{};
    double y{};
    double z{};

    constexpr Vec3() = default;
    constexpr Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    Vec3& operator+=(const Vec3& other) noexcept {
        x += other.x; 
        y += other.y; 
        z += other.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& other) noexcept {
        x -= other.x; 
        y -= other.y; 
        z -= other.z;

        return *this;
    }

    Vec3& operator*=(double s) noexcept {
        x *= s; 
        y *= s; 
        z *= s;

        return *this;
    }

    Vec3& normalize() noexcept {
        const double n2 = x*x + y*y + z*z;
        const double invn = 1.0 / std::sqrt(n2);

        x *= invn; 
        y *= invn; 
        z *= invn;

        return *this;
    }
};

inline Vec3 operator+(Vec3 a, const Vec3& b) noexcept { return a += b; }
inline Vec3 operator-(Vec3 a, const Vec3& b) noexcept { return a -= b; }

inline Vec3 operator*(Vec3 a, double s) noexcept { return a *= s; }
inline Vec3 operator*(double s, Vec3 a) noexcept { return a *= s; }

inline double norm2(const Vec3& v) noexcept {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double norm(const Vec3& v) noexcept {
    return std::sqrt(norm2(v));
}

inline Vec3 cross(Vec3 a, Vec3 b) noexcept {
    return Vec3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
