#version 460 core

layout(location = 0) in float iVertexId;

out vec4 VertexColor;

uniform float pointSize;
uniform float vertexCount;

// drawPoints.obj用
// 赤い点のグリッド
// 参考: https://www.youtube.com/watch?v=mOEbXQWtP3M
void main() {
    float down = floor(sqrt(vertexCount));
    float across = floor(vertexCount / down);

    float x = mod(iVertexId, across);
    float y = floor(iVertexId / across);

    float u = x / (across - 1.0);
    float v = y / (across - 1.0);

    float ux = u * 2.0 - 1.0;
    float vy = v * 2.0 - 1.0;

    vec2 xy = vec2(ux, vy);

    gl_Position = vec4(xy, 0.0, 1.0);
    gl_PointSize = pointSize;
    VertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
