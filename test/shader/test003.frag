#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;

// 上は黒、下は白のグラデーション
void main() {
    float a = gl_FragCoord.y / resolution.y;
    FragColor = vec4(vec3(a), 1.0);
}
