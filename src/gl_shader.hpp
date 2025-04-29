#pragma once

#include <string>
#include <filesystem>
#include <functional>

#include <glad/gl.h>
#include <lua.hpp>

#include "gl_uniform.hpp"

namespace glshaderkit {

class GLShader {
public:
    GLShader(const std::string& path);
    ~GLShader() {
        Release();
    }

    // コピー禁止
    GLShader(const GLShader&) = delete;
    GLShader& operator=(const GLShader&) = delete;

    // ムーブ
    GLShader(GLShader&& other);
    GLShader& operator=(GLShader&& other);

    void Use() const {
        glUseProgram(program_);
    }

    GLint GetUniformLocation(const char* name) const {
        return glGetUniformLocation(program_, name);
    }

private:
    static std::string LoadShaderSource(const std::filesystem::path& basePath);
    static GLuint CompileShader(const char* source, GLenum type);

    void AttachShaderIfExists(const std::filesystem::path& path, GLenum type, const char* defaultSource = nullptr);

    void Release();

    GLuint program_;
};

namespace lua {

// Programのメタテーブル名
constexpr const char* kProgramMetaName = "GLShaderKit.Program";

inline GLShader* GetLuaProgram(lua_State* L, int index) {
    return *static_cast<GLShader**>(luaL_checkudata(L, index, kProgramMetaName));
}

// Programメタテーブルの登録
void RegisterProgram(lua_State* L);

// Program作成
int CreateProgram(lua_State* L);

int ProgramUse(lua_State* L);
int ProgramSetFloat(lua_State* L);
int ProgramSetInt(lua_State* L);
int ProgramSetUInt(lua_State* L);
int ProgramSetMatrix(lua_State* L);

} // namespace lua

} // namespace glshaderkit
