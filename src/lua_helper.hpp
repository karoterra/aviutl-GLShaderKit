#pragma once

#include <vector>

#include <glad/gl.h>
#include <lua.hpp>

namespace glshaderkit::lua {

// スタックトップのテーブルにLuaに新しいクラステーブルを登録する
void RegisterLuaClassTable(lua_State* L, const char* name, const luaL_Reg* staticMethod);

// Luaに新しいメタテーブルを登録する
void RegisterMetaTable(
    lua_State* L,
    const char* name,
    const luaL_Reg* metaMethod,
    const luaL_Reg* method
);

GLenum CheckDrawMode(lua_State* L, int narg);

GLfloat LuaToFloat(lua_State* L, int index);
GLint LuaToInt(lua_State* L, int index);
GLuint LuaToUInt (lua_State* L, int index);

std::vector<float> LuaTableToVector(lua_State* L, int index);

} // namespace glshaderkit::lua
