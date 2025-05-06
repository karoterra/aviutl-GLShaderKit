#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D texture0;
uniform float time;
uniform vec4 track;

float rand(vec2 st){ return fract(sin(dot(st.xy ,vec2(12.9898,78.233))) * 43758.5453); }
float blockNoise(vec2 st) { return rand(floor(st)); }

// グリッチ
// 参考: https://zenn.dev/umeyan/articles/e312dd0bd8a61f
void main() {
    float blockScale = track.x;
    float noiseSpeed = track.y;
    float glitchIntensity = track.z;

    float noise = blockNoise(vec2(TexCoord.y * blockScale));
    noise += rand(vec2(TexCoord.x)) * 0.3;
    float rd = blockNoise(vec2(TexCoord.y, time * noiseSpeed)) * 2.0 - 1.0;
    vec2 uv = TexCoord;
    uv.x += rd * sin(sin(glitchIntensity)*0.5) * sin(-sin(noise)*0.2) * fract(time);

    FragColor.r = texture(texture0, uv + vec2(0.006, 0.0)).r;
    FragColor.g = texture(texture0, uv).g;
    FragColor.b = texture(texture0, uv - vec2(0.008, 0.0)).b;
    FragColor.a = 1.0;
}
