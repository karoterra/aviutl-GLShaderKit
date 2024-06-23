#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;

// 左は黒、右は白のグラデーション
void main() {
    float a = gl_FragCoord.x / resolution.x;
    FragColor = vec4(vec3(a), 1.0);
}
