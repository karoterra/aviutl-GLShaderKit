#version 460 core

layout(binding = 0) uniform sampler2D texture0;

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;
uniform float shift;

const float PI = 3.141592653589793;

// dgree から radian に変換する
float deg2rad(float deg) {
    return deg / 180.0 * PI;
}

// ベクトルvを回転させる
vec2 rotate(vec2 v, float deg) {
    float rad = deg2rad(deg);
    float c = cos(rad);
    float s = sin(rad);
    mat2 m = mat2(c, -s, s, c);
    return m * v;
}

// 色ずれさせる
void main() {
    vec2 unit = vec2(1.0, 0.0);
    vec2 uv1 = TexCoord + rotate(unit, 70.0) * shift / resolution;
    vec2 uv2 = TexCoord + rotate(unit, 190.0) * shift / resolution;
    vec2 uv3 = TexCoord + rotate(unit, -50.0) * shift / resolution;
    vec4 c1 = texture(texture0, uv1);
    vec4 c2 = texture(texture0, uv2);
    vec4 c3 = texture(texture0, uv3);
    FragColor = vec4(c1.r, c2.g, c3.b, 1.0);
}
