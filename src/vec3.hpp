#pragma once
#include <cmath>

/*
Vec3 type for forces, positions and velocities
*/

struct Vec3 {
    double x{};
    double y{};
    double z{};

    Vec3() = default;

    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Vec3& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    Vec3& normalize();
};

inline Vec3 operator+(Vec3 a, const Vec3& b) {
    return a += b;
}

inline Vec3 operator-(Vec3 a, const Vec3& b) {
    return a -= b;
}

inline Vec3 operator*(Vec3 a, double scalar) {
    return a *= scalar;
}

inline Vec3 operator*(double scalar, Vec3 a) {
    return a *= scalar;
}

inline double norm2(const Vec3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double norm(const Vec3& v) {
    return std::sqrt(norm2(v));
}

inline Vec3 cross(Vec3 a, Vec3 b){
    return Vec3 {
        a.y*b.z - a.z*b.y,
        b.x*a.z - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

inline Vec3& Vec3::normalize(){
        float n = norm(*this);
        x *= 1.0/n;
        y *= 1.0/n;
        z *= 1.0/n;
        return *this;
}