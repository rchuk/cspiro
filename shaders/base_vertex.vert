#version 330 core

layout (location = 0) in vec4 vertex_pos;
layout (location = 1) in vec4 vertex_color;

out VertexData {
    vec4 color;
} vertex;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    vertex.color = vertex_color;
    gl_Position = projection_matrix * view_matrix * model_matrix * vertex_pos;
}
