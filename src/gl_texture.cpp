#include "gl_texture.hpp"

namespace glshaderkit {

GLTexture::GLTexture(const void* data, int width, int height)
    : texture_(0), width_(width), height_(height) {
    Initialize(data);
}

GLTexture::GLTexture(GLTexture&& other)
    : texture_(other.texture_), width_(other.width_), height_(other.height_) {
    other.texture_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) {
    if (this == &other) {
        return *this;
    }

    Release();
    texture_ = other.texture_;
    width_ = other.width_;
    height_ = other.height_;

    other.texture_ = 0;
    other.width_ = 0;
    other.height_ = 0;

    return *this;
}

void GLTexture::Initialize(const void* data) {
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
    glTextureStorage2D(texture_, 1, GL_RGBA8, width_, height_);
    glTextureSubImage2D(texture_, 0, 0, 0, width_, height_, GL_BGRA, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // GPUへのアップロード完了待ち
    GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    GLenum waitResult = GL_UNSIGNALED;
    while (waitResult != GL_ALREADY_SIGNALED && waitResult != GL_CONDITION_SATISFIED) {
        waitResult = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
    }
    glDeleteSync(sync);
}

void GLTexture::Release() {
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
}

} // namespace glshaderkit
