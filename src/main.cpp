#include <lua.hpp>

#include "log.hpp"

#define GL_SHADER_KIT_API extern "C" __declspec(dllexport)

#ifndef GL_SHADER_KIT_VERSION
#   define GL_SHADER_KIT_VERSION "0.0.0"
#endif

int version(lua_State* L) {
    lua_pushstring(L, GL_SHADER_KIT_VERSION);
    return 1;
}

static const luaL_Reg kLibFunctions[] = {
    {"version", version},
    {nullptr, nullptr},
};

GL_SHADER_KIT_API int luaopen_GLShaderKit(lua_State * L) {
    luaL_register(L, "GLShaderKit", kLibFunctions);
    return 1;
}
