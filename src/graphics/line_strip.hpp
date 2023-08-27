#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "GL/GL.h"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "vertex_2d.hpp"


class LineStrip {
public:
    const std::vector<Vertex2D>& get_vertices() const {
        return vertices;
    }

    void push_point(glm::vec2 pos, float width, glm::vec4 color);

private:
    std::size_t point_count = 0;
    float prev_width = 1.0f;
    glm::vec2 prev_point;
    glm::vec2 prev_prev_point;
    glm::vec4 prev_color;
    std::vector<Vertex2D> vertices;

private:
    void on_new_point(glm::vec2 pos, float width, glm::vec4 color);

    glm::vec2 get_cap_offset(glm::vec2 pos, float width);
    glm::vec2 get_joint_offset(glm::vec2 pos);
};

class LineStripRenderer {
public:
    ~LineStripRenderer();

    static std::unique_ptr<LineStripRenderer> create();

    LineStrip& get_strip() {
        return strip;
    }

    void render();

private:
    GLuint vao;
    GLuint vbo;

    LineStrip strip;
    size_t prev_vertex_count = 0;

    // TODO: Don't hardcode it
    // NOTE: Roughly 1mb buffer
    constexpr static size_t max_vertex_count = 44000;

private:
    LineStripRenderer(GLuint vao, GLuint vbo) : vao(vao), vbo(vbo) {}

    void flush();
};
