#pragma once

#include "math/vector.hpp"

struct Body {
    Vector position;
    Vector velocity;
    Vector acceleration;
    float mass;

    Body(const Vector& position, float mass) : position(position), mass(mass) {}
};

