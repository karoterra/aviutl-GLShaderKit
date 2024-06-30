#version 460 core

in vec4 VertexColor;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;

// drawPoints.obj用
void main() {
    FragColor = VertexColor;
}
