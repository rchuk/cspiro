#pragma once

#include <cstddef>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


struct Vertex2D {
public:
    glm::vec2 pos;
    glm::vec4 col;

    Vertex2D(glm::vec2 pos, glm::vec4 col) : pos(pos), col(col) {}

    constexpr static size_t get_pos_offset() {
        return offsetof(Vertex2D, pos);
    }

    constexpr static size_t get_col_offset() {
        return offsetof(Vertex2D, col);
    }
};
