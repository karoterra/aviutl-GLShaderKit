#version 460 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec2 resolution;
uniform float time;
uniform float totalTime;
uniform int frame;
uniform int totalFrame;
uniform vec4 track;

uniform float f1;
uniform vec2 f2;
uniform vec3 f3;
uniform vec4 f4;

uniform int i1;
uniform ivec2 i2;
uniform ivec3 i3;
uniform ivec4 i4;

uniform uint ui1;
uniform uvec2 ui2;
uniform uvec3 ui3;
uniform uvec4 ui4;

uniform mat2 m2;
uniform mat2 m2t;
uniform mat3 m3;
uniform mat4 m4;
uniform mat2x3 m2x3;
uniform mat3x2 m3x2;
uniform mat2x4 m2x4;
uniform mat4x2 m4x2;
uniform mat3x4 m3x4;
uniform mat4x3 m4x3;

const vec3 BLACK = vec3(0.0, 0.0, 0.0);
const vec3 WHITE = vec3(1.0, 1.0, 1.0);
const vec2 SQUARE_SIZE = vec2(0.1, 0.1);

// 長方形の外側ならプラス、内側ならマイナス
float sdBox(vec2 p, vec2 size) {
    vec2 d = abs(p) - size;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// 水平バー
float sdHBar(vec2 p, vec2 size, float rate) {
    vec2 size1 = vec2(mix(0.0, size.x, rate), size.y);
    vec2 p1 = vec2(p.x + mix(size.x, 0.0, rate), p.y);
    return sdBox(p1, size1);
}

// 垂直バー
float sdVBar(vec2 p, vec2 size, float rate) {
    vec2 size1 = vec2(size.x, mix(0.0, size.y, rate));
    vec2 p1 = vec2(p.x, p.y - mix(size.y, 0.0, rate));
    return sdBox(p1, size1);
}

// 水平線
float sdHLine(vec2 p, float thick) {
    return abs(p.y) - thick;
}

// 垂直線
float sdVLine(vec2 p, float thick) {
    return abs(p.x) - thick;
}

//
vec2 gridXY(vec2 p, vec2 xy) {
    return p - vec2(-0.9) - xy * 0.2;
}

void main() {
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec3 color = BLACK;

    // グリッド (0, 0) time / totalTime
    color = mix(vec3(1.0, 0.0, 0.0), color, step(0.0, sdHBar(gridXY(p, vec2(0.0, 0.0)), SQUARE_SIZE, time / totalTime)));
    // グリッド (1, 0) frame / totalFrame
    color = mix(vec3(1.0, 0.0, 0.0), color, step(0.0, sdVBar(gridXY(p, vec2(1.0, 0.0)), SQUARE_SIZE, float(frame) / float(totalFrame))));

    // グリッド (0, 1) track0
    color = mix(vec3(0.0, 1.0, 0.0), color, step(0.0, sdVBar(gridXY(p, vec2(0.0, 1.0)), SQUARE_SIZE, track.x / 10000.0)));
    // グリッド (1, 1) track1
    color = mix(vec3(0.0, 1.0, 0.0), color, step(0.0, sdVBar(gridXY(p, vec2(1.0, 1.0)), SQUARE_SIZE, track.y / 10000.0)));
    // グリッド (2, 1) track2
    color = mix(vec3(0.0, 1.0, 0.0), color, step(0.0, sdVBar(gridXY(p, vec2(2.0, 1.0)), SQUARE_SIZE, track.z / 10000.0)));
    // グリッド (3, 1) track3
    color = mix(vec3(0.0, 1.0, 0.0), color, step(0.0, sdVBar(gridXY(p, vec2(3.0, 1.0)), SQUARE_SIZE, track.w / 10000.0)));

    // グリッド (0, 2) f1
    color = mix(vec3(f1), color, step(0.0, sdBox(gridXY(p, vec2(0.0, 2.0)), SQUARE_SIZE)));
    // グリッド (1, 2) f2
    color = mix(vec3(f2, 0.0), color, step(0.0, sdBox(gridXY(p, vec2(1.0, 2.0)), SQUARE_SIZE)));
    // グリッド (2, 2) f3
    color = mix(f3, color, step(0.0, sdBox(gridXY(p, vec2(2.0, 2.0)), SQUARE_SIZE)));
    // グリッド (3, 2) f4.xyz
    color = mix(f4.xyz, color, step(0.0, sdBox(gridXY(p, vec2(3.0, 2.0)), SQUARE_SIZE)));
    // グリッド (4, 2) f4.w
    color = mix(vec3(f4.w), color, step(0.0, sdBox(gridXY(p, vec2(4.0, 2.0)), SQUARE_SIZE)));

    // グリッド (0, 3) i1
    color = mix(vec3(i1) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(0.0, 3.0)), SQUARE_SIZE)));
    // グリッド (1, 3) i2
    color = mix(vec3(i2, 0.0) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(1.0, 3.0)), SQUARE_SIZE)));
    // グリッド (2, 3) i3
    color = mix(i3 / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(2.0, 3.0)), SQUARE_SIZE)));
    // グリッド (3, 3) i4.xyz
    color = mix(i4.xyz / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(3.0, 3.0)), SQUARE_SIZE)));
    // グリッド (4, 3) i4.w
    color = mix(vec3(i4.w) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(4.0, 3.0)), SQUARE_SIZE)));

    // グリッド (0, 4) ui1
    color = mix(vec3(ui1) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(0.0, 4.0)), SQUARE_SIZE)));
    // グリッド (1, 4) ui2
    color = mix(vec3(ui2, 0.0) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(1.0, 4.0)), SQUARE_SIZE)));
    // グリッド (2, 4) ui3
    color = mix(ui3 / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(2.0, 4.0)), SQUARE_SIZE)));
    // グリッド (3, 4) ui4.xyz
    color = mix(ui4.xyz / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(3.0, 4.0)), SQUARE_SIZE)));
    // グリッド (4, 4) ui4.w
    color = mix(vec3(ui4.w) / 255.0, color, step(0.0, sdBox(gridXY(p, vec2(4.0, 4.0)), SQUARE_SIZE)));

    // グリッド (0, 5) m2
    color = mix(vec3(m2[0], 0.0), color, step(0.0, sdBox(gridXY(p, vec2(0.0, 5.0)), SQUARE_SIZE)));
    // グリッド (1, 5) m2
    color = mix(vec3(m2[1], 0.0), color, step(0.0, sdBox(gridXY(p, vec2(1.0, 5.0)), SQUARE_SIZE)));
    // グリッド (2, 5) m2t
    color = mix(vec3(m2t[0], 0.0), color, step(0.0, sdBox(gridXY(p, vec2(2.0, 5.0)), SQUARE_SIZE)));
    // グリッド (3, 5) m2t
    color = mix(vec3(m2t[1], 0.0), color, step(0.0, sdBox(gridXY(p, vec2(3.0, 5.0)), SQUARE_SIZE)));

    // グリッド (0, 6) m3
    color = mix(m3[0], color, step(0.0, sdBox(gridXY(p, vec2(0.0, 6.0)), SQUARE_SIZE)));
    // グリッド (1, 6) m3
    color = mix(m3[1], color, step(0.0, sdBox(gridXY(p, vec2(1.0, 6.0)), SQUARE_SIZE)));
    // グリッド (2, 6) m3
    color = mix(m3[2], color, step(0.0, sdBox(gridXY(p, vec2(2.0, 6.0)), SQUARE_SIZE)));

    // グリッド (0, 7) m4
    color = mix(vec3(m4[0].x, m4[0].y, m4[0].z), color, step(0.0, sdBox(gridXY(p, vec2(0.0, 7.0)), SQUARE_SIZE)));
    // グリッド (1, 7) m4
    color = mix(vec3(m4[0].w, m4[1].x, m4[1].y), color, step(0.0, sdBox(gridXY(p, vec2(1.0, 7.0)), SQUARE_SIZE)));
    // グリッド (2, 7) m4
    color = mix(vec3(m4[1].z, m4[1].w, m4[2].x), color, step(0.0, sdBox(gridXY(p, vec2(2.0, 7.0)), SQUARE_SIZE)));
    // グリッド (3, 7) m4
    color = mix(vec3(m4[2].y, m4[2].z, m4[2].w), color, step(0.0, sdBox(gridXY(p, vec2(3.0, 7.0)), SQUARE_SIZE)));
    // グリッド (4, 7) m4
    color = mix(vec3(m4[3].x, m4[3].y, m4[3].z), color, step(0.0, sdBox(gridXY(p, vec2(4.0, 7.0)), SQUARE_SIZE)));

    // グリッド (0, 8) m2x3
    color = mix(m2x3[0].xyz, color, step(0.0, sdBox(gridXY(p, vec2(0.0, 8.0)), SQUARE_SIZE)));
    // グリッド (1, 8) m2x3
    color = mix(m2x3[1].xyz, color, step(0.0, sdBox(gridXY(p, vec2(1.0, 8.0)), SQUARE_SIZE)));
    // グリッド (2, 8) m3x2
    color = mix(vec3(m3x2[0].xy, m3x2[1].x), color, step(0.0, sdBox(gridXY(p, vec2(2.0, 8.0)), SQUARE_SIZE)));
    // グリッド (3, 8) m3x2
    color = mix(vec3(m3x2[1].y, m3x2[2].xy), color, step(0.0, sdBox(gridXY(p, vec2(3.0, 8.0)), SQUARE_SIZE)));

    // グリッド (0, 9) m2x4
    color = mix(vec3(m2x4[0].x, m2x4[0].y, m2x4[0].z), color, step(0.0, sdBox(gridXY(p, vec2(0.0, 9.0)), SQUARE_SIZE)));
    // グリッド (1, 9) m2x4
    color = mix(vec3(m2x4[0].w, m2x4[1].x, m2x4[1].y), color, step(0.0, sdBox(gridXY(p, vec2(1.0, 9.0)), SQUARE_SIZE)));
    // グリッド (2, 9) m2x4, m4x2
    color = mix(vec3(m2x4[1].z, m2x4[1].w, m4x2[0].x), color, step(0.0, sdBox(gridXY(p, vec2(2.0, 9.0)), SQUARE_SIZE)));
    // グリッド (3, 9) m4x2
    color = mix(vec3(m4x2[0].y, m4x2[1].x, m4x2[1].y), color, step(0.0, sdBox(gridXY(p, vec2(3.0, 9.0)), SQUARE_SIZE)));
    // グリッド (4, 9) m4x2
    color = mix(vec3(m4x2[2].x, m4x2[2].y, m4x2[3].x), color, step(0.0, sdBox(gridXY(p, vec2(4.0, 9.0)), SQUARE_SIZE)));

    // グリッド (5, 0) m3x4
    color = mix(vec3(m3x4[0].x, m3x4[0].y, m3x4[0].z), color, step(0.0, sdBox(gridXY(p, vec2(5.0, 0.0)), SQUARE_SIZE)));
    // グリッド (6, 0) m3x4
    color = mix(vec3(m3x4[0].w, m3x4[1].x, m3x4[1].y), color, step(0.0, sdBox(gridXY(p, vec2(6.0, 0.0)), SQUARE_SIZE)));
    // グリッド (7, 0) m3x4
    color = mix(vec3(m3x4[1].z, m3x4[1].w, m3x4[2].x), color, step(0.0, sdBox(gridXY(p, vec2(7.0, 0.0)), SQUARE_SIZE)));
    // グリッド (8, 0) m3x4
    color = mix(vec3(m3x4[2].y, m3x4[2].z, m3x4[2].w), color, step(0.0, sdBox(gridXY(p, vec2(8.0, 0.0)), SQUARE_SIZE)));

    // グリッド (5, 1) m4x3
    color = mix(m4x3[0].xyz, color, step(0.0, sdBox(gridXY(p, vec2(5.0, 1.0)), SQUARE_SIZE)));
    // グリッド (6, 1) m4x3
    color = mix(m4x3[1].xyz, color, step(0.0, sdBox(gridXY(p, vec2(6.0, 1.0)), SQUARE_SIZE)));
    // グリッド (7, 1) m4x3
    color = mix(m4x3[2].xyz, color, step(0.0, sdBox(gridXY(p, vec2(7.0, 1.0)), SQUARE_SIZE)));
    // グリッド (8, 1) m4x3
    color = mix(m4x3[3].xyz, color, step(0.0, sdBox(gridXY(p, vec2(8.0, 1.0)), SQUARE_SIZE)));

    // グリッド (10x10)
    color = mix(vec3(0.5, 0.5, 0.5), color, step(0.0, sdHLine(fract(p * 5.0), 0.05)));
    color = mix(vec3(0.5, 0.5, 0.5), color, step(0.0, sdVLine(fract(p * 5.0), 0.05)));

    FragColor = vec4(color, 1.0);
}
