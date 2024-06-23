#pragma once

#include <glad/gl.h>

namespace glshaderkit {

class GLTexture {
public:
    GLTexture(const void* data, int width, int height);
    ~GLTexture() {
        Release();
    }

    // コピー禁止
    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;

    // ムーブ
    GLTexture(GLTexture&& other);
    GLTexture& operator=(GLTexture&& other);

    void Bind(int unit) const {
        glBindTextureUnit(unit, texture_);
    }

    static void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    void Initialize(const void* data);
    void Release();

    GLuint texture_;
    int width_;
    int height_;
};

} // namespace glshaderkit
