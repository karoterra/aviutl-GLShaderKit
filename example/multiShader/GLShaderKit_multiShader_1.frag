#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;
uniform float size;
uniform mat3 affine;

// 長方形のSDF
float sdBox(vec2 p, vec2 size) {
    vec2 d = abs(p) - size;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// 図形の描画
void main() {
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    p = (affine * vec3(p, 1.0)).xy;

    float color = 1.0 - step(0.0, sdBox(p, vec2(size)));

    FragColor = vec4(vec3(color), 1.0);
}
