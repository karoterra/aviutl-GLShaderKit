#pragma once

#include <glad/gl.h>

namespace glshaderkit {

class GLFramebuffer {
public:
    GLFramebuffer(GLsizei width, GLsizei height)
        : fbo_(0), texture_(0), width_(width), height_(height) {
        Initialize();
    }

    ~GLFramebuffer() {
        Release();
    }

    // コピー禁止
    GLFramebuffer(const GLFramebuffer&) = delete;
    GLFramebuffer& operator=(const GLFramebuffer&) = delete;

    // ムーブ
    GLFramebuffer(GLFramebuffer&& other);
    GLFramebuffer& operator=(GLFramebuffer&& other);

    void Resize(GLsizei width, GLsizei height);

    void Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glViewport(0, 0, width_, height_);
    }

    static void Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLsizei Width() const {
        return width_;
    }

    GLsizei Height() const {
        return height_;
    }

private:
    void Initialize();
    void Release();

    GLuint fbo_;
    GLuint texture_;
    GLsizei width_;
    GLsizei height_;

};

} // namespace glshaderkit
