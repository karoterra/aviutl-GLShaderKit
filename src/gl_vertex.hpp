#pragma once

#include <glad/gl.h>
#include <lua.hpp>

#include "release.hpp"

namespace glshaderkit {

class GLVertex : public IRelease {
public:
    enum class Primitive{
        Plane,
        Points,
    };

    GLVertex(int n);
    GLVertex(Primitive primitive, int n);
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

    void Release() override;

private:
    void Initialize(int n);

    GLuint vao_;
    GLuint vbo_;
    GLuint ibo_;
    Primitive primitive_;
    int size_;
    int indexCount_;
};

namespace lua {

// Vertexのメタテーブル名
constexpr const char* kVertexMetaName = "GLShaderKit.Vertex";

inline GLVertex* GetLuaVertex(lua_State* L, int index) {
    return *static_cast<GLVertex**>(luaL_checkudata(L, index, kVertexMetaName));
}

// Vertexメタテーブルの登録
void RegisterVertex(lua_State* L);

// Vertex作成
int CreateVertex(lua_State* L);

// ガベージコレクションメタメソッド
int VertexMetaGC(lua_State* L);

// バインド
int VertexBind(lua_State* L);
// バインド解除
int VertexUnbind(lua_State* L);
// VAOをフレームバッファに描画
int VertexDraw(lua_State* L);
// リリース
int VertexRelease(lua_State* L);

} // namespace lua

} // namespace glshaderkit
