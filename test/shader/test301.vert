#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 TexCoord;

uniform int instanceCount;

// effectInstanced.anm用
void main() {
    float down = floor(sqrt(float(instanceCount)));
    float across = floor(float(instanceCount) / down);

    float x = mod(float(gl_InstanceID), across);
    float y = floor(float(gl_InstanceID) / across);

    vec3 scale = vec3(1.0 / across);
    vec3 offset = vec3(
        -1.0 + scale.x + x * scale.x * 2.0,
        -1.0 + scale.y + y * scale.y * 2.0,
        0.0
    );

    vec3 pos = iPos * scale + offset;

    gl_Position = vec4(pos, 1.0);
    TexCoord = iTexCoord;
}
