#version 330 core

out vec4 frag_color;

in VertexData {
    vec4 color;
} vertex;

void main() {
    frag_color = vec4(vertex.color);
}
