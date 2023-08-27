#include "shader.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


Shader::~Shader() {
    glDeleteShader(shader);
}

std::unique_ptr<Shader> Shader::load(GLenum kind, const std::string& filename) {
    const GLuint shader = glCreateShader(kind);
    if (!shader) {
        std::cout << "Couldn't create shader\n";
    
        return nullptr;
    }

    std::ifstream stream(filename);
    if (!stream) {
        std::cout << "Couldn't load shader source from \"" << filename << "\"\n";

        glDeleteShader(shader);

        return nullptr;
    }

    std::stringstream buffer;
    buffer << stream.rdbuf();
    const std::string source = buffer.str();
    const char* const source_c_str = source.c_str();

    glShaderSource(shader, 1, &source_c_str, NULL);
    glCompileShader(shader);

    int is_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_success);
    if (!is_success) {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> error_log(max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, error_log.data());
        std::cout << "Shader compilation failed:\n" << error_log.data() << "\n";

        glDeleteShader(shader);

        return nullptr;
    }

    return std::unique_ptr<Shader>(new Shader(shader));
}
