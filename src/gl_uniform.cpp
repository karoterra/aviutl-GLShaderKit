#include "gl_uniform.hpp"

#include <string>

#include "lua_helper.hpp"

namespace glshaderkit {

namespace lua {

int SetUniformMatrix(lua_State* L, GLint location, int offset) {
    int top = lua_gettop(L);
    std::string matrixType = luaL_checkstring(L, offset + 1);
    bool transpose = lua_toboolean(L, offset + 2);

    if (!lua_istable(L, offset + 3)) {
        return luaL_error(L, "引数に配列が指定されていません");
    }

    auto matrix = LuaTableToVector(L, offset + 3);
    auto it = kGLMatrixSetterMap.find(matrixType);
    if (it == kGLMatrixSetterMap.end()) {
        return luaL_error(L, "'%s' は無効な行列の型です", matrixType.c_str());
    }

    const auto& setter = it->second;
    if (matrix.size() != setter.size) {
        return luaL_error(L,
            "行列の要素数が無効です。 (期待値: %d, 実際の値: %d)",
            setter.size, matrix.size());
    }

    setter.setter(location, 1, transpose, matrix.data());
    return 0;
}

} // namespace lua

} // namespace glshaderkit
