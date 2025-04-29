#include "gl_framebuffer.hpp"

#include <glad/gl.h>

#include "gl_context.hpp"
#include "lua_helper.hpp"
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
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

namespace lua {

void RegisterFrameBuffer(lua_State* L) {
    const luaL_Reg metaMethod[] = {
        {"__gc", FrameBufferMetaGC},
        {nullptr, nullptr},
    };
    const luaL_Reg method[] = {
        {"bind", FrameBufferBind},
        {"unbind", FrameBufferUnbind},
        {"bindTexture", FrameBufferBindTexture},
        {"readPixels", FrameBufferReadPixels},
        {"release", FrameBufferRelease},
        {nullptr, nullptr},
    };

    RegisterMetaTable(L, kFrameBufferMetaName, metaMethod, method);
}

int CreateFrameBuffer(lua_State* L) {
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    GLFramebuffer* fbo = new GLFramebuffer(width, height);
    GLContext::Instance().GetReleaseContainer().Add(fbo);

    GLFramebuffer** udata = static_cast<GLFramebuffer**>(lua_newuserdata(L, sizeof(GLFramebuffer*)));
    *udata = fbo;
    luaL_getmetatable(L, kFrameBufferMetaName);
    lua_setmetatable(L, -2);
    return 1;
}

int FrameBufferMetaGC(lua_State* L) {
    GLFramebuffer* self = GetLuaFrameBuffer(L, 1);
    GLContext::Instance().GetReleaseContainer().Remove(self);
    delete self;
    return 0;
}

int FrameBufferBind(lua_State* L) {
    GLFramebuffer* self = GetLuaFrameBuffer(L, 1);
    self->Bind();
    return 0;
}

int FrameBufferUnbind(lua_State* L) {
    GLFramebuffer::Unbind();
    return 0;
}

int FrameBufferBindTexture(lua_State* L) {
    GLFramebuffer* self = GetLuaFrameBuffer(L, 1);
    int unit = luaL_checkinteger(L, 2);
    self->BindTexture(unit);
    return 0;
}

int FrameBufferReadPixels(lua_State* L) {
    GLFramebuffer* self = GetLuaFrameBuffer(L, 1);
    void* data = lua_touserdata(L, 2);
    self->Bind();
    glReadPixels(0, 0, self->Width(), self->Height(), GL_BGRA, GL_UNSIGNED_BYTE, data);
    return 0;
}

int FrameBufferRelease(lua_State* L) {
    GLFramebuffer* self = GetLuaFrameBuffer(L, 1);
    self->Release();
    return 0;
}

} // namespace lua


} // namespace glshaderkit
