#pragma once

#include <glad/gl.h>
#include <lua.hpp>

#include "release.hpp"

namespace glshaderkit {

class GLTexture : public IRelease {
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

    void Release() override;

private:
    void Initialize(const void* data);

    GLuint texture_;
    int width_;
    int height_;
};

namespace lua {

// Textureのメタテーブル名
constexpr const char* kTextureMetaName = "GLShaderKit.Texture";

inline GLTexture* GetLuaTexture(lua_State* L, int index) {
    return *static_cast<GLTexture**>(luaL_checkudata(L, index, kTextureMetaName));
}

// Textureメタテーブルの登録
void RegisterTexture(lua_State* L);

// Texture作成
int TextureNew(lua_State* L);

// ガベージコレクションメタメソッド
int TextureMetaGC(lua_State* L);

// バインド
int TextureBind(lua_State* L);
// バインド解除
int TextureUnbind(lua_State* L);
// リリース
int TextureRelease(lua_State* L);

} // namespace lua

} // namespace glshaderkit
