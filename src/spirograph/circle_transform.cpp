#include "circle_transform.hpp"


void CircleTransform::rotate(const CircleTransform& parent, float angle_delta) {
    const float radius_ratio = radius / parent.radius;

    rotation += angle_delta;
    position.angle += angle_delta * radius_ratio;
}