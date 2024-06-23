#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;

// 赤と緑のグラデーション
// 中心を円で切り抜き
void main() {
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec2 color = (vec2(1.0) + p.xy) * 0.5;
    float alpha = smoothstep(0.45, 0.55, dot(p, p));
    FragColor = vec4(color, 0.0, alpha);
}
