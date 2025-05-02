#pragma once

#include <glad/gl.h>
#include <lua.hpp>

#include "release.hpp"

namespace glshaderkit {

class GLFramebuffer : public IRelease {
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

    void BindTexture(int unit) const {
        glBindTextureUnit(unit, texture_);
    }

    GLsizei Width() const {
        return width_;
    }

    GLsizei Height() const {
        return height_;
    }

    void Release() override;

private:
    void Initialize();

    GLuint fbo_;
    GLuint texture_;
    GLsizei width_;
    GLsizei height_;
};

namespace lua {

// FrameBufferのメタテーブル名
constexpr const char* kFrameBufferMetaName = "GLShaderKit.FrameBuffer";

inline GLFramebuffer* GetLuaFrameBuffer(lua_State* L, int index) {
    return *static_cast<GLFramebuffer**>(luaL_checkudata(L, index, kFrameBufferMetaName));
}

// FrameBufferメタテーブルの登録
void RegisterFrameBuffer(lua_State* L);

// FrameBuffer作成
int FrameBufferNew(lua_State* L);

// ガベージコレクションメタメソッド
int FrameBufferMetaGC(lua_State* L);

// バインド
int FrameBufferBind(lua_State* L);
// バインド解除
int FrameBufferUnbind(lua_State* L);
// カラーバッファをテクスチャとしてバインド
int FrameBufferBindTexture(lua_State* L);
// カラーバッファのデータを取得
int FrameBufferReadPixels(lua_State* L);
// リリース
int FrameBufferRelease(lua_State* L);

} // namespace lua

} // namespace glshaderkit
