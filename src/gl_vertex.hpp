#pragma once

#include <glad/gl.h>

namespace glshaderkit {

class GLVertex {
public:
    enum class Primitive{
        Plane,
        Points,
    };

    GLVertex(int n);
    ~GLVertex() {
        Release();
    }

    // コピー禁止
    GLVertex(const GLVertex&) = delete;
    GLVertex& operator=(const GLVertex&) = delete;

    // ムーブ
    GLVertex(GLVertex&& other);
    GLVertex& operator=(GLVertex&& other);

    void Bind() const {
        glBindVertexArray(vao_);
    }

    static void Unbind() {
        glBindVertexArray(0);
    }

    void SetPlane(int n);
    void SetPoints(int n);

    Primitive GetPrimitive() const {
        return primitive_;
    }

    int Size() const {
        return size_;
    }

    int IndexCount() const {
        return indexCount_;
    }

private:
    void Initialize(int n);
    void Release();

    GLuint vao_;
    GLuint vbo_;
    GLuint ibo_;
    Primitive primitive_;
    int size_;
    int indexCount_;
};

} // namespace glshaderkit
