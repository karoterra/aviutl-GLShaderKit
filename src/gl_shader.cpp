#include "gl_shader.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <stdexcept>

#include "gl_context.hpp"
#include "lua_helper.hpp"
#include "log.hpp"

namespace glshaderkit {

namespace fs = std::filesystem;

const char* kDefaultVertexShaderSource = R"(
#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(iPos, 1.0);
    TexCoord = iTexCoord;
}
)";

GLShader::GLShader(const std::string& basePath) : program_(0) {
    program_ = glCreateProgram();

    try {
        fs::path p = basePath;
        AttachShaderIfExists(p.replace_extension(".vert"), GL_VERTEX_SHADER, kDefaultVertexShaderSource);
        AttachShaderIfExists(p.replace_extension(".frag"), GL_FRAGMENT_SHADER);
        AttachShaderIfExists(p.replace_extension(".tesc"), GL_TESS_CONTROL_SHADER);
        AttachShaderIfExists(p.replace_extension(".tese"), GL_TESS_EVALUATION_SHADER);
        AttachShaderIfExists(p.replace_extension(".geom"), GL_GEOMETRY_SHADER);

        Log::Info("シェーダーをリンクします。");
        glLinkProgram(program_);

        GLint status = GL_FALSE;
        glGetProgramiv(program_, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            GLint length = 0;
            glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);

            std::vector<GLchar> infoLog(length);
            glGetProgramInfoLog(program_, length, nullptr, infoLog.data());

            throw std::runtime_error(infoLog.data());
        }
    }
    catch (const std::runtime_error& e) {
        glDeleteProgram(program_);
        throw;
    }
}

GLShader::GLShader(GLShader&& other) : program_(other.program_) {
    other.program_ = 0;
}

GLShader& GLShader::operator=(GLShader&& other) {
    if (this == &other) {
        return *this;
    }

    Release();

    program_ = other.program_;
    other.program_ = 0;

    return *this;
}

std::string GLShader::LoadShaderSource(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("ファイルを開けませんでした: " + path.string());
    }

    std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return src;
}

GLuint GLShader::CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(length);
        glGetShaderInfoLog(shader, length, nullptr, infoLog.data());
        glDeleteShader(shader);

        throw std::runtime_error(infoLog.data());
    }

    return shader;
}

void GLShader::AttachShaderIfExists(const std::filesystem::path& path, GLenum type, const char* defaultSource) {
    GLuint shader;

    if (fs::exists(path)) {
        Log::Info("シェーダーをコンパイルします: " + path.string());
        std::string source = LoadShaderSource(path);
        shader = CompileShader(source.c_str(), type);
    }
    else if (defaultSource != nullptr) {
        Log::Info("シェーダー '" + path.string() + "'が見つからなかったため、デフォルトシェーダーを使用します。");
        shader = CompileShader(defaultSource, type);
    }
    else {
        return;
    }

    glAttachShader(program_, shader);
    glDeleteShader(shader);
}

void GLShader::Release() {
    if (program_ != 0) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

namespace lua {

void RegisterProgram(lua_State* L) {
    const luaL_Reg staticMethod[] = {
        {"new", ProgramNew},
        {nullptr, nullptr},
    };
    const luaL_Reg metaMethod[] = {
        {nullptr, nullptr},
    };
    const luaL_Reg method[] = {
        {"use", ProgramUse},
        {"setFloat", ProgramSetFloat},
        {"setInt", ProgramSetInt},
        {"setUInt", ProgramSetUInt},
        {"setMatrix", ProgramSetMatrix},
        {nullptr, nullptr},
    };

    RegisterLuaClassTable(L, "Program", staticMethod);
    RegisterMetaTable(L, kProgramMetaName, metaMethod, method);
}

int ProgramNew(lua_State* L) {
    // 第1引数はクラステーブル
    const char* path = luaL_checkstring(L, 2);
    const bool force = lua_toboolean(L, 3);
    auto shader = GLContext::Instance().GetShaderManager().CreateProgram(path, force);

    GLShader** udata = static_cast<GLShader**>(lua_newuserdata(L, sizeof(GLShader*)));
    *udata = &shader.get();
    luaL_getmetatable(L, kProgramMetaName);
    lua_setmetatable(L, -2);
    return 1;
}

int ProgramUse(lua_State* L) {
    GLShader* self = GetLuaProgram(L, 1);
    self->Use();
    return 0;
}

int ProgramSetFloat(lua_State* L) {
    GLShader* self = GetLuaProgram(L, 1);
    const char* name = luaL_checkstring(L, 2);
    auto location = self->GetUniformLocation(name);
    return SetUniform<GLfloat>(L, location, 2, LuaToFloat, kGLUniformFloatSetter);
}

int ProgramSetInt(lua_State* L) {
    GLShader* self = GetLuaProgram(L, 1);
    const char* name = luaL_checkstring(L, 2);
    auto location = self->GetUniformLocation(name);
    return SetUniform<GLint>(L, location, 2, LuaToInt, kGLUniformIntSetter);
}

int ProgramSetUInt(lua_State* L) {
    GLShader* self = GetLuaProgram(L, 1);
    const char* name = luaL_checkstring(L, 2);
    auto location = self->GetUniformLocation(name);
    return SetUniform<GLuint>(L, location, 2, LuaToUInt, kGLUniformUIntSetter);
}

int ProgramSetMatrix(lua_State* L) {
    GLShader* self = GetLuaProgram(L, 1);
    const char* name = luaL_checkstring(L, 2);
    auto location = self->GetUniformLocation(name);
    return SetUniformMatrix(L, location, 2);
}

} // namespace lua

} // namespace glshaderkit
