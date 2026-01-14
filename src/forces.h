#pragma once 
#include "vec3.hpp"
#include "config.h"

const double softening = 1e-5; // Avoid division by 0 

enum InteractionType {
    GRAVITY,
    ELECTRIC
};

class ForceField {
    public:
    InteractionType type;
    virtual Vec3 force() {return Vec3(0,0,0);};
    virtual Vec3 gravity(float m1, float m2, Vec3 r){return Vec3(0,0,0);};
    virtual Vec3 electric(float m1, float m2, Vec3 r){return Vec3(0,0,0);};
};

class Gravity : public ForceField
{
    public:
    const InteractionType type = GRAVITY;
};

class ClassicalGravity : public Gravity
{
    public: 
    Vec3 gravity(float m1, float m2, Vec3 r){
        double dist2 = norm2(r) + softening * softening;
        double dist = std::sqrt(dist2);
        if (dist == 0.0) {
            return Vec3(0,0,0);
        }
        double f = G * m1 * m2 * 1.0 / (dist2 * dist);
        return -f * r;
    }    
};

class R3Gravity : public Gravity
{
    Vec3 gravity(float m1, float m2, Vec3 r) override
    {
        double dist2 = norm2(r) + softening * softening;
        double dist = std::sqrt(dist2);
        if (dist == 0.0) {
            return Vec3(0,0,0);
        }
        double f = G * m1 * m2 * 1.0 / (dist2 * dist2);
        return -f * r;
    }
};

class QuantizedGravity : public Gravity
{
    float f_quanta=0.01;
    Vec3 gravity(float m1, float m2, Vec3 r) override
    {
        double dist2 = norm2(r) + softening * softening;
        double dist = std::sqrt(dist2);

        if (dist == 0.0) {
            return Vec3(0,0,0);
        }
        double f = G * m1 * m2 * 1.0 / (dist2 * dist);
        if (f < f_quanta){ f = f_quanta; }
        return -f * r;
    }
};

class Electric : public ForceField
{
    const InteractionType type = ELECTRIC;
    public:
    Vec3 electric(float q1, float q2, Vec3 r)
    {
        double dist2 = norm2(r) + softening * softening;
        double dist = std::sqrt(dist2);

        if (dist == 0.0) {
            return Vec3(0,0,0);
        }
        double f = K * q1 * q2 * 1.0 / (dist2 * dist);
        return f * r;
    }
};