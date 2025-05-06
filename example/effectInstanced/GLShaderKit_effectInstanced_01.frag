#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D texture0;

// effectInstanced.anm用
// ネガ
void main() {
    vec4 color = texture(texture0, TexCoord);
    color.rgb = 1.0 - color.rgb;
    FragColor = color;
}
