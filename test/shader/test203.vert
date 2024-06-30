#version 460 core

layout(location = 0) in float iVertexId;

out vec4 VertexColor;

uniform float time;
uniform float pointSize;
uniform float vertexCount;

// 参考: https://gist.github.com/983/e170a24ae8eba2cd174f
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// drawPoints.obj用
// 時間に応じて頂点位置と色が変わる
// 参考: https://www.youtube.com/watch?v=9fgGnWdObN4
void main() {
    float down = floor(sqrt(vertexCount));
    float across = floor(vertexCount / down);

    float x = mod(iVertexId, across);
    float y = floor(iVertexId / across);

    float u = x / (across - 1.0);
    float v = y / (across - 1.0);

    float xoff = sin(time + y * 0.2) * 0.1;
    float yoff = sin(time + x * 0.3) * 0.2;

    float ux = u * 2.0 - 1.0 + xoff;
    float vy = v * 2.0 - 1.0 + yoff;

    vec2 xy = vec2(ux, vy) * 1.3;

    float ps = sin(time + x * y * 0.02) * 5.0 + 15.0;
    ps *= pointSize / 10.0;

    float hue = u * 0.1 + sin(time + v * 20.0) * 0.05;
    float sat = 1.0;
    float val = sin(time + v * u * 20.0) * 0.5 + 0.5;

    gl_Position = vec4(xy, 0.0, 1.0);
    gl_PointSize = ps;
    VertexColor = vec4(hsv2rgb(vec3(hue, sat, val)), 1.0);
}
