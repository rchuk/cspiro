#pragma once

#include <string>
#include <memory>

#include "GL/glew.h"
#include "GL/GL.h"


class Shader {
public:
    ~Shader();

    GLuint get_handle() const {
        return shader;
    }

    static std::unique_ptr<Shader> load(GLenum kind, const std::string& filename);

private:
    const GLuint shader;

private:
    Shader(GLuint shader) : shader(shader) {}
};
