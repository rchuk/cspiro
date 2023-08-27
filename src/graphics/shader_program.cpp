#include "shader_program.hpp"

#include <iostream>


ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program);
}

std::unique_ptr<ShaderProgram> ShaderProgram::create(const std::vector<Shader>& shaders) {
    const GLuint program = glCreateProgram();
    if (!program) {
        std::cout << "Couldn't create shader program\n";

        return {};
    }

    for (const auto& shader : shaders)
        glAttachShader(program, shader.get_handle());

    glLinkProgram(program);

    int is_success;
    glGetProgramiv(program, GL_LINK_STATUS, &is_success);
    if (!is_success) {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> error_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, error_log.data());
        std::cout << "Shader program linking failed:\n" << error_log.data() << "\n";

        glDeleteProgram(program);

        return nullptr;
    }

    return std::unique_ptr<ShaderProgram>(new ShaderProgram(program));
}
