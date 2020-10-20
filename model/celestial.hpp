#pragma once

#include "model/body.hpp"

namespace minima {

struct OrbitalMotion {
    float altitude;
    float horizontal_speed;
    float vertical_speed;
};

struct Celestial : Body {
    float radius;
    float gravity;

    Celestial(const Vector& position, float mass, float radius, float gravity) : Body(position, mass), radius(radius), gravity(gravity) {}

    OrbitalMotion get_orbital_motion(const Body& body) const {
        Vector difference = body.position - position;
        Vector normal = difference.perpendicular().unit();
        Vector unit = difference.unit();
        return { position.distance(body.position) - radius, normal.x * velocity.x + normal.y * velocity.y, unit.y * velocity.y + unit.x * velocity.x };
    }
};

}

