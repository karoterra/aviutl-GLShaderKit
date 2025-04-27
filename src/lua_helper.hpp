#pragma once

#include <glad/gl.h>
#include <lua.hpp>

namespace glshaderkit::lua {

// Luaに新しいメタテーブルを登録する
void RegisterMetaTable(
    lua_State* L,
    const char* name,
    const luaL_Reg* metaMethod,
    const luaL_Reg* method
);

GLenum CheckDrawMode(lua_State* L, int narg);

} // namespace glshaderkit::lua
