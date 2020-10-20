#pragma once

#include <cmath>

struct Vector {
    float x;
    float y;

    Vector() : x(0.0f), y(0.0f) {}
    Vector(float x, float y) : x(x), y(y) {}

    Vector operator+(const Vector& right) const {
        return { x + right.x, y + right.y };
    }

    Vector operator-(const Vector& right) const {
        return { x - right.x, y - right.y };
    }

    Vector operator*(const Vector& right) const {
        return { x * right.x, y * right.y };
    }

    Vector operator/(const Vector& right) const {
        return { x / right.x, y / right.y };
    }

    Vector operator+(float right) const {
        return { x + right, y + right };
    }

    Vector operator-(float right) const {
        return { x - right, y - right };
    }

    Vector operator*(float right) const {
        return { x * right, y * right };
    }

    Vector operator/(float right) const {
        return { x / right, y / right };
    }

    Vector perpendicular() const {
        return { y, -x };
    }

    Vector unit() const {
        float length = sqrt(pow(x, 2.0f) + pow(y, 2.0f));
        return { x / length, y / length };
    }

    Vector rotated(float rotation) const {
        return { x * cos(rotation) - y * sin(rotation), x * sin(rotation) + y * cos(rotation) };
    }

    float distance(const Vector& to) const {
        return sqrt(pow(to.x - x, 2.0f) + pow(to.y - y, 2.0f));
    }
};
