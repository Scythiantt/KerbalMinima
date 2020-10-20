#pragma once

#include "model/celestial.hpp"
#include "math/vector.hpp"

namespace minima {

struct Planet : Celestial {

    Planet(const Vector& position, float mass, float radius, float gravity) : Celestial(position, mass, radius, gravity) {}

    void apply_gravity(Body& body) const {
        Vector unit = (position - body.position).unit();
        body.acceleration = body.acceleration + unit * gravity;
    }
};

}