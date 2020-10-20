#pragma once

#include <queue>

#include "model/spacecraft.hpp"
#include "model/planet.hpp"
#include "math/vector.hpp"
#include "math/range.hpp"

namespace minima {

struct TrajectoryNode {
    float thrust_gimbal;
    TrajectoryNode* previous;
};

struct Trajectory {
    std::vector<TrajectoryNode> nodes;
    float time_step;
};

struct OrbitalMotionMap {
    // TODO: Implement n-dimensional array
    bool*** map;
    bool out_of_bounds = true;

    Range altitude;
    Range horizontal_speed;
    Range vertical_speed;

    OrbitalMotionMap(Range altitude, Range horizontal_speed, Range vertical_speed) : altitude(altitude), horizontal_speed(horizontal_speed), vertical_speed(vertical_speed) {
        int sizes[] = { altitude.size + 1, horizontal_speed.size + 1, vertical_speed.size + 1 };
        map = new bool** [sizes[0]];
        for (int i = 0; i < sizes[0]; i++) {
            map[i] = new bool* [sizes[1]];
            for (int j = 0; j < sizes[1]; j++) {
                map[i][j] = new bool[sizes[2]];
                for (int k = 0; k < sizes[2]; k++) {
                    map[i][j][k] = false;
                }
            }
        }
    }

    bool& at(const OrbitalMotion& motion) {
        if (!altitude.in_range(motion.altitude) || !horizontal_speed.in_range(motion.horizontal_speed) || !vertical_speed.in_range(motion.vertical_speed)) return out_of_bounds;
        return map[altitude[motion.altitude]][horizontal_speed[motion.horizontal_speed]][vertical_speed[motion.vertical_speed]];
    }
};

namespace suborbital {

struct Objective {
    Vector location;
};

struct Cost {
    float fuel_spent;
};

struct FlightNode {
    Spacecraft spacecraft;
    Cost cost;
    TrajectoryNode* trajectory_node;
};

Trajectory optimal_trajectory(const Spacecraft& spacecraft, const Planet& planet, const Objective& objective) {
    return optimal_trajectory_impl(spacecraft, planet, objective);
}

Trajectory optimal_trajectory_impl(Spacecraft spacecraft, Planet planet, const Objective& objective) {
    // TODO: Improve greedy strategy
    // fuel spent^2 / distance to target
    auto comparator = [&objective](const FlightNode& left, const FlightNode& right) {
        return left.spacecraft.position.distance(objective.location) < right.spacecraft.position.distance(objective.location);
    };
    // TODO: Consider using stack allocation insteap of a heap
    std::priority_queue<FlightNode, std::vector<FlightNode>, decltype(comparator)> flight_queue(comparator);

    // TODO: Same goes here
    const int MAX_TRAJECTORY_NODES = 1000000;
    std::vector<TrajectoryNode> trajectory_nodes(MAX_TRAJECTORY_NODES);

    flight_queue.push({ spacecraft, Cost(), nullptr });

    while (!flight_queue.empty()) {
        FlightNode flight_node = flight_queue.top();
        flight_queue.pop();

        // TODO: What if a spacecraft overflies the objective?
        if (flight_node.spacecraft.position.distance(objective.location) <= 1000.0f) {
            Trajectory trajectory;
            TrajectoryNode* current = flight_node.trajectory_node;
            while (current != nullptr) {
                trajectory.nodes.insert(trajectory.nodes.begin(), *current);
                current = current->previous;
            }
            return trajectory;
        }

        for (int i = 0; i < 16; i++) {
            FlightNode next_flight_node = flight_node;
            float angle = i * 0.10472f;

            trajectory_nodes.push_back({ angle, flight_node.trajectory_node });
            next_flight_node.trajectory_node = &trajectory_nodes.back();

            for (int t = 0; t < 10; t++) {
                planet.apply_gravity(next_flight_node.spacecraft);
                next_flight_node.spacecraft.thrust_gimbal = (next_flight_node.spacecraft.position - planet.position).perpendicular().unit().rotated(angle);
                next_flight_node.spacecraft.update(1.0f);
            }

            flight_queue.push(next_flight_node);
        }
    }
}

}

namespace orbital {

struct Objective {
    float parameter;
};

struct Cost {
    bool orbit_achieved;
};

struct FlightNode {
    Spacecraft spacecraft;
    Cost cost;
};

Trajectory compute_trajectory(const Spacecraft& spacecraft, const Planet& planet, const Objective& objective) {
    // TODO: Consider using stack allocation insteap of a heap
    std::vector<float> trajectory(1024);
    // TODO: Compute the size of motion map based on objective
    OrbitalMotionMap motion_map({ 0.0f, 100000.0f, 100 }, { 0.0f, 5000.0f, 100 }, { -5000.0f, 5000.0f, 100 });

    compute_trajectory_impl({ spacecraft, Cost() }, planet, trajectory, objective, motion_map);

    Trajectory final_trajectory;
    final_trajectory.time_step = 10.0f;
    for (float gimbal : trajectory) {
        final_trajectory.nodes.push_back({ gimbal, nullptr });
    }
    
    return final_trajectory;
}

// TODO: Parameterize number of update iterations and range of angles
Cost compute_trajectory_impl(FlightNode flight_node, const Planet& planet, std::vector<float>& trajectory, const Objective& objective, OrbitalMotionMap& motion_map) {
    bool& motion = motion_map.at(planet.get_orbital_motion(flight_node.spacecraft));
    if (motion) return { false };

    for (int i = flight_node.spacecraft.has_thrust ? 0 : 15; i < 16; i++) {
        float angle = i * 0.10472f;
        trajectory.push_back(angle);

        FlightNode next_flight_node = flight_node;
        for (int t = 0; t < 10; t++) {
            planet.apply_gravity(next_flight_node.spacecraft);
            next_flight_node.spacecraft.thrust_gimbal = (next_flight_node.spacecraft.position - planet.position).perpendicular().unit().rotated(angle);
            next_flight_node.spacecraft.update(1.0f);
        }

        Cost cost = compute_trajectory_impl(next_flight_node, planet, trajectory, objective, motion_map);
        if (cost.orbit_achieved) return cost;
        motion = true;

        trajectory.pop_back();
    }

    return { false };
}

}

}