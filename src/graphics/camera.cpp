#include "camera.hpp"

#include "glm/gtx/string_cast.hpp"


void Camera::translate(glm::vec3 delta) {
    pos += delta;

    update_view_matrix();
}

void Camera::set_position(glm::vec3 new_pos) {
    pos = new_pos;

    update_view_matrix();
    update_projection_matrix();
}

void Camera::set_zoom(float new_zoom) {
    zoom = new_zoom;

    update_view_matrix();
} 

void Camera::set_near_clipping_plane(float distance) {
    near_clipping = distance;

    update_projection_matrix();
}

void Camera::set_far_clipping_plane(float distance) {
    far_clipping = distance;

    update_projection_matrix();
}

void Camera::on_viewport_change(glm::vec2 new_viewport_size) {
    viewport_size = new_viewport_size;

    update_projection_matrix();
}

void Camera::update_view_matrix() {
    // TODO: Implent rotation using quaternion
    view_matrix = glm::scale(glm::lookAt(
        pos,
        pos + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    ), glm::vec3(zoom));
}

void Camera::update_projection_matrix() {
    projection_matrix = glm::ortho(
        - 0.5f * viewport_size.x,
        0.5f * viewport_size.x,
        - 0.5f * viewport_size.y,
        0.5f * viewport_size.y,
        near_clipping,
        far_clipping
    );
}
