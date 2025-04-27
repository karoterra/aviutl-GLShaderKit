#include <Windows.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#include <glad/gl.h>
#include <lua.hpp>

#include "gl_context.hpp"
#include "gl_texture.hpp"
#include "gl_framebuffer.hpp"
#include "gl_vertex.hpp"
#include "lua_helper.hpp"
#include "config.hpp"
#include "log.hpp"

#define GL_SHADER_KIT_API extern "C" __declspec(dllexport)

#ifndef GL_SHADER_KIT_VERSION
#   define GL_SHADER_KIT_VERSION "0.0.0"
#endif

std::vector<float> LuaTableToVector(lua_State* L, int index) {
    std::vector<float> result;
    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        if (lua_isnumber(L, -1)) {
            result.push_back(static_cast<float>(lua_tonumber(L, -1)));
        }
        lua_pop(L, 1);
    }
    return result;
}

int version(lua_State* L) {
    lua_pushstring(L, GL_SHADER_KIT_VERSION);
    return 1;
}

int isInitialized(lua_State* L) {
    lua_pushboolean(L, glshaderkit::GLContext::Instance().IsInitialized());
    return 1;
}

int glVendor(lua_State* L) {
    const auto s = glshaderkit::GLContext::Instance().GetString(GL_VENDOR);
    lua_pushstring(L, s);
    return 1;
}

int glRenderer(lua_State* L) {
    const auto s = glshaderkit::GLContext::Instance().GetString(GL_RENDERER);
    lua_pushstring(L, s);
    return 1;
}

int glVersion(lua_State* L) {
    const auto s = glshaderkit::GLContext::Instance().GetString(GL_VERSION);
    lua_pushstring(L, s);
    return 1;
}

int glslVersion(lua_State* L) {
    const auto s = glshaderkit::GLContext::Instance().GetString(GL_SHADING_LANGUAGE_VERSION);
    lua_pushstring(L, s);
    return 1;
}

int activate(lua_State* L) {
    bool ok = glshaderkit::GLContext::Instance().Activate();
    lua_pushboolean(L, ok);
    return 1;
}

int deactivate(lua_State* L) {
    glshaderkit::GLContext::Instance().Deactivate();
    return 0;
}

int setPlaneVertex(lua_State* L) {
    if (lua_gettop(L) < 1) {
        return luaL_error(L, "setPlaneVertex()には引数が1個必要です");
    }
    int n = lua_tointeger(L, 1);
    glshaderkit::GLContext::Instance().SetPlaneVertex(n);
    return 0;
}

int setPointVertex(lua_State* L) {
    if (lua_gettop(L) < 1) {
        return luaL_error(L, "setPointVertex()には引数が1個必要です");
    }
    int n = lua_tointeger(L, 1);
    glshaderkit::GLContext::Instance().SetPointVertex(n);
    return 0;
}

int setShader(lua_State* L) {
    if (lua_gettop(L) < 1) {
        return luaL_error(L, "setShader()には引数が1個必要です");
    }
    const char* path = lua_tostring(L, 1);
    bool forceReload = lua_toboolean(L, 2);
    glshaderkit::GLContext::Instance().SetShader((path ? path : ""), forceReload);
    return 0;
}

int draw(lua_State* L) {
    if (lua_gettop(L) < 4) {
        return luaL_error(L, "draw()には引数が4個必要です");
    }
    GLenum mode = glshaderkit::lua::CheckDrawMode(L, 1);
    void* data = lua_touserdata(L, 2);
    int w = lua_tointeger(L, 3);
    int h = lua_tointeger(L, 4);
    int instanceCount = lua_tointeger(L, 5);

    glshaderkit::GLContext::Instance().Draw(mode, data, w, h, instanceCount);
    return 0;
}

int setFloat(lua_State* L) {
    int top = lua_gettop(L);
    if (top < 2) {
        return luaL_error(L, "setFloat()には引数が2から5個の引数が必要です");
    }
    const char* name = lua_tostring(L, 1);
    auto& context = glshaderkit::GLContext::Instance();
    auto location = context.GetUniformLocation(name);
    switch (top) {
    case 2:
        glUniform1f(location, lua_tonumber(L, 2));
        break;
    case 3:
        glUniform2f(location, lua_tonumber(L, 2), lua_tonumber(L, 3));
        break;
    case 4:
        glUniform3f(location, lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
        break;
    case 5:
        glUniform4f(location, lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
        break;
    }
    return 0;
}

int setInt(lua_State* L) {
    int top = lua_gettop(L);
    if (top < 2) {
        return luaL_error(L, "setInt()には引数が2から5個の引数が必要です");
    }
    const char* name = lua_tostring(L, 1);
    auto& context = glshaderkit::GLContext::Instance();
    auto location = context.GetUniformLocation(name);
    switch (top) {
    case 2:
        glUniform1i(location, lua_tointeger(L, 2));
        break;
    case 3:
        glUniform2i(location, lua_tointeger(L, 2), lua_tointeger(L, 3));
        break;
    case 4:
        glUniform3i(location, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4));
        break;
    case 5:
        glUniform4i(location, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
        break;
    }
    return 0;
}

int setUInt(lua_State* L) {
    int top = lua_gettop(L);
    if (top < 2) {
        return luaL_error(L, "setUInt()には引数が2から5個の引数が必要です");
    }
    const char* name = lua_tostring(L, 1);
    auto& context = glshaderkit::GLContext::Instance();
    auto location = context.GetUniformLocation(name);
    switch (top) {
    case 2:
        glUniform1ui(location, lua_tointeger(L, 2));
        break;
    case 3:
        glUniform2ui(location, lua_tointeger(L, 2), lua_tointeger(L, 3));
        break;
    case 4:
        glUniform3ui(location, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4));
        break;
    case 5:
        glUniform4ui(location, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
        break;
    }
    return 0;
}

int setMatrix(lua_State* L) {
    int top = lua_gettop(L);

    const char* name = lua_tostring(L, 1);
    const char* matrixType = lua_tostring(L, 2);
    const bool transpose = lua_toboolean(L, 3);
    if (!lua_istable(L, 4)) {
        return luaL_error(L, "第4引数には配列を指定してください");
    }
    auto matrix = LuaTableToVector(L, 4);

    auto& context = glshaderkit::GLContext::Instance();
    auto location = context.GetUniformLocation(name);

    if (std::strcmp(matrixType, "2x2") == 0) {
        if (matrix.size() != 4) return luaL_error(L, "配列の要素数が4ではありません");
        glUniformMatrix2fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "3x3") == 0) {
        if (matrix.size() != 9) return luaL_error(L, "配列の要素数が9ではありません");
        glUniformMatrix3fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "4x4") == 0) {
        if (matrix.size() != 16) return luaL_error(L, "配列の要素数が16ではありません");
        glUniformMatrix4fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "2x3") == 0) {
        if (matrix.size() != 6) return luaL_error(L, "配列の要素数が6ではありません");
        glUniformMatrix2x3fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "3x2") == 0) {
        if (matrix.size() != 6) return luaL_error(L, "配列の要素数が6ではありません");
        glUniformMatrix3x2fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "2x4") == 0) {
        if (matrix.size() != 8) return luaL_error(L, "配列の要素数が8ではありません");
        glUniformMatrix2x4fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "4x2") == 0) {
        if (matrix.size() != 8) return luaL_error(L, "配列の要素数が8ではありません");
        glUniformMatrix4x2fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "3x4") == 0) {
        if (matrix.size() != 12) return luaL_error(L, "配列の要素数が12ではありません");
        glUniformMatrix3x4fv(location, 1, transpose, matrix.data());
    }
    else if (std::strcmp(matrixType, "4x3") == 0) {
        if (matrix.size() != 12) return luaL_error(L, "配列の要素数が12ではありません");
        glUniformMatrix4x3fv(location, 1, transpose, matrix.data());
    }
    return 0;
}

int setTexture2D(lua_State* L) {
    if (lua_gettop(L) < 4) {
        return luaL_error(L, "setTexture2D()には引数が4個必要です");
    }
    int unit = lua_tointeger(L, 1);
    void* data = lua_touserdata(L, 2);
    int width = lua_tointeger(L, 3);
    int height = lua_tointeger(L, 4);
    glshaderkit::GLContext::Instance().SetTexture2D(unit, data, width, height);
    return 0;
}

// ガベージコレクションメタメソッド
int glshaderkit_gc(lua_State* L) {
    glshaderkit::GLContext::Instance().Release();
    return 0;
}

static const luaL_Reg kLibFunctions[] = {
    {"version", version},
    {"isInitialized", isInitialized},
    {"glVendor", glVendor},
    {"glRenderer", glRenderer},
    {"glVersion", glVersion},
    {"glslVersion", glslVersion},
    {"activate", activate},
    {"deactivate", deactivate},
    {"setPlaneVertex", setPlaneVertex},
    {"setPointVertex", setPointVertex},
    {"setShader", setShader},
    {"draw", draw},
    {"setFloat", setFloat},
    {"setInt", setInt},
    {"setUInt", setUInt},
    {"setMatrix", setMatrix},
    {"setTexture2D", setTexture2D},
    {"createTexture", glshaderkit::lua::CreateTexture},
    {"createFrameBuffer", glshaderkit::lua::CreateFrameBuffer},
    {"createVertex", glshaderkit::lua::CreateVertex},
    {nullptr, nullptr},
};

// Luaモジュールとしてロードされたときの処理
GL_SHADER_KIT_API int luaopen_GLShaderKit(lua_State* L) {
    auto& context = glshaderkit::GLContext::Instance();
    try {
        glshaderkit::Config config;
        // 設定ファイルが見つかれば読み込む
        HMODULE hmod = GetModuleHandleA("GLShaderKit.dll");
        char dllPath[MAX_PATH];
        if (GetModuleFileNameA(hmod, dllPath, MAX_PATH)) {
            std::filesystem::path configPath(dllPath);
            configPath.replace_extension(".ini");
            if (std::filesystem::exists(configPath)) {
                config.Load(configPath.string().c_str());
            }
        }

        context.Initialize(config);
        if (!context.IsInitialized()) {
            glshaderkit::Log::Error("初期化に失敗しました");
            context.Release();
        }
    }
    catch (const std::runtime_error& e) {
        glshaderkit::Log::Error("初期化に失敗しました");
        glshaderkit::Log::Error(e.what());
        context.Release();
    }

    // クラスのメタテーブル登録
    glshaderkit::lua::RegisterTexture(L);
    glshaderkit::lua::RegisterFrameBuffer(L);
    glshaderkit::lua::RegisterVertex(L);

    // Luaステートが閉じるときにGLコンテキストを解放させる
    void* ud = lua_newuserdata(L, 1);
    if (!ud) {
        return luaL_error(L, "lua_newuserdata failed");
    }
    luaL_newmetatable(L, "GLShaderKit.GC");
    lua_pushcfunction(L, glshaderkit_gc);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
    lua_setfield(L, LUA_REGISTRYINDEX, "GLShaderKit.GCHandle");

    // モジュールメソッド登録
    luaL_register(L, "GLShaderKit", kLibFunctions);

    return 1;
}
