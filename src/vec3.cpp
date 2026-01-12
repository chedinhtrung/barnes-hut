#include "vec3.hpp"
Vec3 operator+(Vec3 a, const Vec3& b) {
    return a += b;
}

Vec3 operator-(Vec3 a, const Vec3& b) {
    return a -= b;
}

Vec3 operator*(Vec3 a, double scalar) {
    return a *= scalar;
}

Vec3 operator*(double scalar, Vec3 a) {
    return a *= scalar;
}

double norm2(const Vec3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double norm(const Vec3& v) {
    return std::sqrt(norm2(v));
}

Vec3 cross(Vec3 a, Vec3 b){
    return Vec3 {
        a.y*b.z - a.z*b.y,
        b.x*a.z - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

Vec3& Vec3::normalize(){
        float n = norm(*this);
        x *= 1.0/n;
        y *= 1.0/n;
        z *= 1.0/n;
        return *this;
}