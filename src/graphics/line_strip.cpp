#include "line_strip.hpp"

#include "GL/glew.h"
#include "GL/GL.h"

#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"


void LineStrip::push_point(glm::vec2 pos, float width, glm::vec4 color) {
    if (point_count == 0) {
        on_new_point(pos, width, color);

        return;
    }
    
    const glm::vec2 start = prev_point;
    const glm::vec2 end = pos;
    const glm::vec2 offset = get_cap_offset(pos, width);

    if (point_count == 1) {
        vertices.insert(vertices.end(), { 
            { start + offset, prev_color },
            { start - offset, prev_color },
            { end + offset, color },
            { end - offset, color }
        });
    } else {
        const glm::vec2 joint_offset = get_joint_offset(pos);

        vertices[vertices.size() - 2] = { start + joint_offset, prev_color };
        vertices[vertices.size() - 1] = { start - joint_offset, prev_color };

        vertices.insert(vertices.end(), {
            { end + offset, color },
            { end - offset, color }
        });
    }

    on_new_point(pos, width, color);
}

void LineStrip::on_new_point(glm::vec2 pos, float width, glm::vec4 color) {
    prev_width = width;
    prev_prev_point = prev_point;
    prev_point = pos;
    prev_color = color;

    ++point_count;
}

glm::vec2 LineStrip::get_cap_offset(glm::vec2 pos, float width) {
    const glm::vec2 diff = pos - prev_point;
    const glm::vec2 normal = glm::normalize(glm::vec2(-diff.y, diff.x));

    return normal * width;
}

glm::vec2 LineStrip::get_joint_offset(glm::vec2 pos) {
    const glm::vec2 prev_segment = glm::normalize(prev_point - prev_prev_point);
    const glm::vec2 curr_segment = glm::normalize(pos - prev_point);
    const glm::vec2 joint_dir = glm::normalize(curr_segment - prev_segment);

    const float cos_angle = glm::dot(curr_segment, -prev_segment);
    const float mult = 1.0f / glm::sqrt(0.5f * (1.0f - cos_angle));

    const float wind_mult = glm::cross(glm::vec3(prev_segment, 0.0f), glm::vec3(curr_segment, 0.0f)).z;
    
    return joint_dir * prev_width * mult * wind_mult;
}


LineStripRenderer::~LineStripRenderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

std::unique_ptr<LineStripRenderer> LineStripRenderer::create() {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    if (!vao)
        return nullptr;

    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    if (!vbo) {
        glDeleteVertexArrays(1, &vao);

        return nullptr;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, max_vertex_count * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)Vertex2D::get_pos_offset());
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)Vertex2D::get_col_offset());

    return std::unique_ptr<LineStripRenderer>(new LineStripRenderer(vao, vbo));
}

void LineStripRenderer::render() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    flush();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, strip.get_vertices().size());
}

void LineStripRenderer::flush() {
    const size_t vertex_count = strip.get_vertices().size();
    if (vertex_count == 0 || vertex_count == prev_vertex_count)
        return;

    if (vertex_count > max_vertex_count)
        return;
    
    const size_t new_vertex_count = vertex_count - prev_vertex_count;
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * prev_vertex_count, sizeof(Vertex2D) * new_vertex_count, &strip.get_vertices().data()[prev_vertex_count]);

    prev_vertex_count = vertex_count;
}
