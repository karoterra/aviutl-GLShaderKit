#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D texture0;

uniform vec2 resolution;
uniform vec2 direction;

uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

// ガウシアンブラー
// 参考: https://www.rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
void main() {
    vec4 color = texture(texture0, TexCoord) * weight[0];
    for (int i = 1; i < 3; i++) {
        vec2 uvOffset = vec2(offset[i]) * direction / resolution;
        color += texture(texture0, TexCoord + uvOffset) * weight[i];
        color += texture(texture0, TexCoord - uvOffset) * weight[i];
    }
    FragColor = color;
}
