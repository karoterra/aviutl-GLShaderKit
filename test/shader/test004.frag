#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;
uniform float time;
uniform float totalTime;

// 時間経過に応じて左から右に向かって白色になる
void main() {
    float rate = time / totalTime;
    float pos = gl_FragCoord.x / resolution.x;
    vec4 color = (pos < rate) ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = color;
}
