#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


class Camera {
public:
    Camera() {
        set_position(glm::vec3(0.0f));
    }

    Camera(glm::vec3 pos) {
        set_position(pos);
    }

    const glm::mat4 get_view_matrix() const {
        return view_matrix;
    }

    const glm::mat4 get_projection_matrix() const {
        return projection_matrix;
    }

    glm::vec3 get_pos() const {
        return pos;
    }

    float get_zoom() const {
        return zoom;
    }

    float get_near_clipping_plane() const {
        return near_clipping;
    }

    float get_far_clipping_plane() const {
        return far_clipping;
    }

    void translate(glm::vec3 delta);
    void set_position(glm::vec3 new_pos);
    void set_zoom(float new_zoom);
    void set_near_clipping_plane(float distance);
    void set_far_clipping_plane(float distance);

    void on_viewport_change(glm::vec2 new_viewport_size);

private:
    glm::vec3 pos;

    glm::vec2 viewport_size;
    float zoom = 5.0f;
    float near_clipping = 0.1f;
    float far_clipping = 100.0f;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

private:
    void update_view_matrix();
    void update_projection_matrix();
};