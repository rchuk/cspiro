#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "GL/GL.h"

#include "shader.hpp"


class ShaderProgram {
public:
    ~ShaderProgram();

    static std::unique_ptr<ShaderProgram> create(const std::vector<Shader>& shaders);

    GLuint get_handle() const {
        return program;
    }

private:
    const GLuint program;

private:
    ShaderProgram(GLuint program) : program(program) {}
};
