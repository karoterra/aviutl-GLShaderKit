#include <Windows.h>

#include <string>
#include <unordered_map>
#include <filesystem>

#include <glad/gl.h>
#include <lua.hpp>

#include "gl_context.hpp"
#include "config.hpp"
#include "log.hpp"

#define GL_SHADER_KIT_API extern "C" __declspec(dllexport)

#ifndef GL_SHADER_KIT_VERSION
#   define GL_SHADER_KIT_VERSION "0.0.0"
#endif

HINSTANCE g_hinst = nullptr;

GLenum GetGLDrawMode(const std::string& modeStr) {
    static const std::unordered_map<std::string, GLenum> modeMap = {
        {"POINTS", GL_POINTS},
        {"LINE_STRIP", GL_LINE_STRIP},
        {"LINE_LOOP", GL_LINE_LOOP},
        {"LINES", GL_LINES},
        {"LINE_STRIP_ADJACENCY", GL_LINE_STRIP_ADJACENCY},
        {"LINES_ADJACENCY", GL_LINES_ADJACENCY},
        {"TRIANGLE_STRIP", GL_TRIANGLE_STRIP},
        {"TRIANGLE_FAN", GL_TRIANGLE_FAN},
        {"TRIANGLES", GL_TRIANGLES},
        {"TRIANGLE_STRIP_ADJACENCY", GL_TRIANGLE_STRIP_ADJACENCY},
        {"TRIANGLES_ADJACENCY", GL_TRIANGLES_ADJACENCY},
        {"PATCHES", GL_PATCHES},
    };

    auto it = modeMap.find(modeStr);
    if (it != modeMap.end()) {
        return it->second;
    }
    else {
        return GL_TRIANGLES;
    }
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
    const char* modeStr = luaL_checkstring(L, 1);
    void* data = lua_touserdata(L, 2);
    int w = lua_tointeger(L, 3);
    int h = lua_tointeger(L, 4);
    int instanceCount = lua_tointeger(L, 5);

    GLenum mode = GetGLDrawMode(modeStr);
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
    switch (top) {
    case 2:
        context.SetFloat(name, lua_tonumber(L, 2));
        break;
    case 3:
        context.SetVec2(name, lua_tonumber(L, 2), lua_tonumber(L, 3));
        break;
    case 4:
        context.SetVec3(name, lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
        break;
    case 5:
        context.SetVec4(name, lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
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
    switch (top) {
    case 2:
        context.SetInt(name, lua_tointeger(L, 2));
        break;
    case 3:
        context.SetIVec2(name, lua_tointeger(L, 2), lua_tointeger(L, 3));
        break;
    case 4:
        context.SetIVec3(name, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4));
        break;
    case 5:
        context.SetIVec4(name, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
        break;
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
    {"setTexture2D", setTexture2D},
    {nullptr, nullptr},
};

// Luaモジュールとしてロードされたときの処理
GL_SHADER_KIT_API int luaopen_GLShaderKit(lua_State* L) {
    auto& context = glshaderkit::GLContext::Instance();
    try {
        glshaderkit::Config config;
        // 設定ファイルが見つかれば読み込む
        char dllPath[MAX_PATH];
        if (GetModuleFileNameA(g_hinst, dllPath, MAX_PATH)) {
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

    luaL_register(L, "GLShaderKit", kLibFunctions);
    return 1;
}

// DLLエントリーポイント
BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        g_hinst = hinst;
        break;
    case DLL_PROCESS_DETACH:
        glshaderkit::GLContext::Instance().Release();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
