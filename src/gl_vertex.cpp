#include "gl_vertex.hpp"

#include <vector>

namespace glshaderkit {

GLVertex::GLVertex(int n) : vao_(0), vbo_(0), ibo_(0), size_(0), indexCount_(0) {
    if (n < 1) {
        n = 1;
    }
    Initialize(n);
}

GLVertex::GLVertex(GLVertex&& other)
    : vao_(other.vao_), vbo_(other.vbo_), ibo_(other.ibo_), size_(other.size_), indexCount_(other.indexCount_) {
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ibo_ = 0;
    other.size_ = 0;
    other.indexCount_ = 0;
}

GLVertex& GLVertex::operator=(GLVertex&& other) {
    if (this == &other) {
        return *this;
    }

    Release();
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
    size_ = other.size_;
    indexCount_ = other.indexCount_;

    other.vao_ = 0;
    other.vbo_ = 0;
    other.ibo_ = 0;
    other.size_ = 0;
    other.indexCount_ = 0;

    return *this;
}

void GLVertex::Resize(int n) {
    if (n == size_) {
        return;
    }
    Release();
    Initialize(n);
}

void GLVertex::Initialize(int n) {
    // 頂点座標とテクスチャ座標のデータ生成
    int numVertices = (n + 1) * (n + 1);
    std::vector<float> vertices(numVertices * 5);

    int vertexIndex = 0;
    for (int y = 0; y <= n; y++) {
        for (int x = 0; x <= n; x++) {
            float fx = static_cast<float>(x) / n * 2.0f - 1.0f;
            float fy = static_cast<float>(y) / n * 2.0f - 1.0f;
            float u = static_cast<float>(x) / n;
            float v = static_cast<float>(y) / n;
            vertices[vertexIndex++] = fx;
            vertices[vertexIndex++] = fy;
            vertices[vertexIndex++] = 0.0f;
            vertices[vertexIndex++] = u;
            vertices[vertexIndex++] = v;
        }
    }

    // インデックスデータ生成
    int numTriangles = n * n * 2;
    std::vector<GLuint> indices(numTriangles * 3);

    int index = 0;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            int bottomLeft = y * (n + 1) + x;
            int bottomRight = bottomLeft + 1;
            int topLeft = bottomLeft + n + 1;
            int topRight = topLeft + 1;

            indices[index++] = topLeft;
            indices[index++] = bottomLeft;
            indices[index++] = topRight;

            indices[index++] = topRight;
            indices[index++] = bottomLeft;
            indices[index++] = bottomRight;
        }
    }

    // VBO
    glCreateBuffers(1, &vbo_);
    glNamedBufferData(vbo_, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // IBO
    glCreateBuffers(1, &ibo_);
    glNamedBufferData(ibo_, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // VAO
    glCreateVertexArrays(1, &vao_);
    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(vao_, ibo_);

    // 頂点座標
    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_, 0, 0);

    // テクスチャ座標
    glEnableVertexArrayAttrib(vao_, 1);
    glVertexArrayAttribFormat(vao_, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(vao_, 1, 0);

    size_ = 0;
    indexCount_ = indices.size();
}

void GLVertex::Release() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (ibo_ != 0) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }
    size_ = 0;
    indexCount_ = 0;
}

} // namespace glshaderkit
