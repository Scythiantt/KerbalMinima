#pragma once

#include "model/body.hpp"
#include "model/planet.hpp"
#include "math/vector.hpp"

#define MAX_STAGES 4

struct Stage {
    float thrust;
    float fuel;
    float fuel_flow;

    Stage() : thrust(0.0f), fuel(0.0f), fuel_flow(0.0f) {}
    Stage(float thrust, float fuel, float fuel_flow) : thrust(thrust), fuel(fuel), fuel_flow(fuel_flow) {}
};

struct Spacecraft : Body {
    Stage stages[MAX_STAGES];
    int stage;

    Vector thrust_gimbal;
    bool has_thrust;

    Spacecraft(const Vector& position, float mass, float radius) : Body(position, mass), stage(0), has_thrust(true) {}

    void update(float delta_time) {
        if (has_thrust) {
            acceleration = acceleration + thrust_gimbal * (stages[stage].thrust / mass);
            float delta_fuel = delta_time * stages[stage].fuel_flow;
            mass = mass - delta_fuel;
            stages[stage].fuel = stages[stage].fuel - delta_fuel;
        }

        velocity = velocity + acceleration * delta_time;
        position = position + velocity * delta_time;

        acceleration = { 0.0f, 0.0f };

        if (stages[stage].fuel < 0.0f) {
            --stage;
            if (stage < 0) {
                has_thrust = false;
            }
        }
    }

    void add_stage(const Stage& stage) {

    }
};