#include "gl_framebuffer.hpp"

#include <glad/gl.h>

#include "log.hpp"

namespace glshaderkit {

GLFramebuffer::GLFramebuffer(GLFramebuffer&& other)
    : fbo_(other.fbo_), texture_(other.texture_), width_(other.width_), height_(other.height_) {
    other.fbo_ = 0;
    other.texture_ = 0;
}

GLFramebuffer& GLFramebuffer::operator=(GLFramebuffer&& other) {
    if (this == &other) {
        return *this;
    }

    Release();

    fbo_ = other.fbo_;
    texture_ = other.texture_;
    width_ = other.width_;
    height_ = other.height_;

    other.fbo_ = 0;
    other.texture_ = 0;

    return *this;
}

void GLFramebuffer::Resize(GLsizei width, GLsizei height) {
    if (width == width_ && height == height_) {
        return;
    }

    Release();

    height_ = height;
    width_ = width;
    Initialize();
}

void GLFramebuffer::Initialize() {
    // フレームバッファを作成
    glCreateFramebuffers(1, &fbo_);

    // テクスチャの作成と設定
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
    glTextureStorage2D(texture_, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // フレームバッファにテクスチャをアタッチ
    glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT0, texture_, 0);

    if (glCheckNamedFramebufferStatus(fbo_, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Log::Error("フレームバッファの初期化に問題があります。");
    }
}

void GLFramebuffer::Release() {
    if (fbo_ != 0) {
        glDeleteFramebuffers(1, &fbo_);
        fbo_ = 0;
    }
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
}

} // namespace glshaderkit
