#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D texture0;

uniform vec4 track;

// RGB各チャネルにトラックバーで指定した倍率 (x0 - 2) をかける
void main() {
    vec4 color = texture(texture0, TexCoord);
    vec3 rate = track.xyz / 10000.0 + 1.0;
    color.rgb = color.rgb * rate;
    FragColor = color;
}
