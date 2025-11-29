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

Vec3 operator+(Vec3 a, const Vec3& b);

Vec3 operator-(Vec3 a, const Vec3& b);

Vec3 operator*(Vec3 a, double scalar);

Vec3 operator*(double scalar, Vec3 a);

Vec3 cross(Vec3 a, Vec3 b);

double norm2(const Vec3& v);

double norm(const Vec3& v);

   