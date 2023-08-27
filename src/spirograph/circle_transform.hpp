#pragma once

#include "glm/vec2.hpp"
#include "glm/trigonometric.hpp"


struct PolarCoords {
public:
    constexpr PolarCoords(float angle, float radius)
        : angle(angle), radius(radius) {}

    constexpr glm::vec2 to_cartesian(glm::vec2 center) {
        return glm::vec2(
            center.x + radius * glm::cos(angle),
            center.y + radius * glm::sin(angle)
        );
    }

    float angle;
    float radius;
};

class CircleTransform {
public:
    constexpr CircleTransform(PolarCoords position, float radius, float rotation)
        : position(position), radius(radius), rotation(rotation) {}

    void rotate(const CircleTransform& parent, float angle_delta);

    PolarCoords position;
    float radius;
    float rotation;
};