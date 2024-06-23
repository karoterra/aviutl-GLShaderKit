#include <Windows.h>

#include <lua.hpp>

#include "gl_context.hpp"
#include "log.hpp"

#define GL_SHADER_KIT_API extern "C" __declspec(dllexport)

#ifndef GL_SHADER_KIT_VERSION
#   define GL_SHADER_KIT_VERSION "0.0.0"
#endif

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

int setVertex(lua_State* L) {
    if (lua_gettop(L) < 1) {
        return luaL_error(L, "setVertex()には引数が1個必要です");
    }
    int n = lua_tointeger(L, 1);
    glshaderkit::GLContext::Instance().SetVertex(n);
    return 0;
}

int setShader(lua_State* L) {
    if (lua_gettop(L) < 1) {
        return luaL_error(L, "setShader()には引数が1個必要です");
    }
    const char* path = lua_tostring(L, 1);
    glshaderkit::GLContext::Instance().SetShader((path ? path : ""));
    return 0;
}

int draw(lua_State* L) {
    if (lua_gettop(L) < 3) {
        return luaL_error(L, "draw()には引数が3個必要です");
    }
    void* data = lua_touserdata(L, 1);
    int w = lua_tointeger(L, 2);
    int h = lua_tointeger(L, 3);
    glshaderkit::GLContext::Instance().Draw(data, w, h);
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

static const luaL_Reg kLibFunctions[] = {
    {"version", version},
    {"isInitialized", isInitialized},
    {"glVendor", glVendor},
    {"glRenderer", glRenderer},
    {"glVersion", glVersion},
    {"glslVersion", glslVersion},
    {"activate", activate},
    {"deactivate", deactivate},
    {"setVertex", setVertex},
    {"setShader", setShader},
    {"draw", draw},
    {"setFloat", setFloat},
    {"setInt", setInt},
    {nullptr, nullptr},
};

// Luaモジュールとしてロードされたときの処理
GL_SHADER_KIT_API int luaopen_GLShaderKit(lua_State* L) {
    auto& context = glshaderkit::GLContext::Instance();
    try {
        context.Initialize();
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
