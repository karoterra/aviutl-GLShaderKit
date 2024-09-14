#version 460 core

layout(binding = 0) uniform sampler2D texture0;
layout(binding = 1) uniform sampler2D texture1;
layout(binding = 2) uniform sampler2D texture2;

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

// 加算合成
vec4 blend(vec4 src, vec4 dst) {
    float fs = 1.0;
    float fd = 1 - src.a;
    vec3 b = min(vec3(1.0), src.rgb + dst.rgb);

    float alpha = src.a * fs + dst.a * fd;
    vec3 c = dst.a * b + (1 - dst.a) * src.rgb;
    vec3 color = src.a * fs * c + dst.a * fd * dst.rgb;
    color = (alpha == 0.0) ? vec3(0.0) : (color / alpha);

    return vec4(color, alpha);
}

void main() {
    vec4 c0 = texture(texture0, TexCoord);
    vec4 c1 = texture(texture1, TexCoord);
    vec4 c2 = texture(texture2, TexCoord);

    FragColor = blend(blend(c0, c1), c2);
}
