#include "lua_helper.hpp"

namespace glshaderkit::lua {

void RegisterMetaTable(
    lua_State* L,
    const char* name,
    const luaL_Reg* metaMethod,
    const luaL_Reg* method
) {
    luaL_newmetatable(L, name);

    luaL_register(L, nullptr, metaMethod);

    lua_newtable(L);
    luaL_register(L, nullptr, method);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);
}

GLenum CheckDrawMode(lua_State* L, int narg) {
    const char* texts[] = {
        "POINTS",
        "LINE_STRIP",
        "LINE_LOOP",
        "LINES",
        "LINE_STRIP_ADJACENCY",
        "LINES_ADJACENCY",
        "TRIANGLE_STRIP",
        "TRIANGLE_FAN",
        "TRIANGLES",
        "TRIANGLE_STRIP_ADJACENCY",
        "TRIANGLES_ADJACENCY",
        "PATCHES",
        nullptr,
    };
    const GLenum modes[] = {
        GL_POINTS,
        GL_LINE_STRIP,
        GL_LINE_LOOP,
        GL_LINES,
        GL_LINE_STRIP_ADJACENCY,
        GL_LINES_ADJACENCY,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP_ADJACENCY,
        GL_TRIANGLES_ADJACENCY,
        GL_PATCHES,
    };
    int index = luaL_checkoption(L, narg, "TRIANGLES", texts);
    return modes[index];
}

} // namespace glshaderkit::lua
