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

static const luaL_Reg kLibFunctions[] = {
    {"version", version},
    {"isInitialized", isInitialized},
    {"glVendor", glVendor},
    {"glRenderer", glRenderer},
    {"glVersion", glVersion},
    {"glslVersion", glslVersion},
    {"activate", activate},
    {"deactivate", deactivate},
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
