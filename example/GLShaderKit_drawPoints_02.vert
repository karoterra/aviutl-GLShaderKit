#version 460 core

layout(location = 0) in float iVertexId;

out vec4 VertexColor;

uniform float time;
uniform float pointSize;
uniform float vertexCount;

// drawPoints.obj用
// 赤い点のグリッドが時間に応じて動く
// 参考: https://www.youtube.com/watch?v=ejd2K2rR-Nk
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

    gl_Position = vec4(xy, 0.0, 1.0);
    gl_PointSize = ps;
    VertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
