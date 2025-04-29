#pragma once

#include <string_view>
#include <unordered_map>
#include <functional>

#include <glad/gl.h>
#include <lua.hpp>

namespace glshaderkit {

template<typename T>
using GLUniformFunc1 = void (GLAD_API_PTR *)(GLint, T);
template<typename T>
using GLUniformFunc2 = void (GLAD_API_PTR *)(GLint, T, T);
template<typename T>
using GLUniformFunc3 = void (GLAD_API_PTR *)(GLint, T, T, T);
template<typename T>
using GLUniformFunc4 = void (GLAD_API_PTR *)(GLint, T, T, T, T);

using GLUniformMatrixFunc = void (GLAD_API_PTR *)(GLint, GLsizei, GLboolean, const GLfloat*);

template<typename T>
struct GLUniformSetter {
    std::reference_wrapper<GLUniformFunc1<T>> setter1;
    std::reference_wrapper<GLUniformFunc2<T>> setter2;
    std::reference_wrapper<GLUniformFunc3<T>> setter3;
    std::reference_wrapper<GLUniformFunc4<T>> setter4;
};

struct GLUniformMatrixSetter {
    size_t size;
    std::reference_wrapper<GLUniformMatrixFunc> setter;
};

inline const GLUniformSetter<GLfloat> kGLUniformFloatSetter {
    glUniform1f,
    glUniform2f,
    glUniform3f,
    glUniform4f,
};
inline const GLUniformSetter<GLint> kGLUniformIntSetter {
    glUniform1i,
    glUniform2i,
    glUniform3i,
    glUniform4i,
};
inline const GLUniformSetter<GLuint> kGLUniformUIntSetter {
    glUniform1ui,
    glUniform2ui,
    glUniform3ui,
    glUniform4ui,
};

inline const std::unordered_map<std::string_view, GLUniformMatrixSetter> kGLMatrixSetterMap = {
    {"2x2", {4, glUniformMatrix2fv}},
    {"3x3", {9, glUniformMatrix3fv}},
    {"4x4", {16, glUniformMatrix4fv}},
    {"2x3", {6, glUniformMatrix2x3fv}},
    {"3x2", {6, glUniformMatrix3x2fv}},
    {"2x4", {8, glUniformMatrix2x4fv}},
    {"4x2", {8, glUniformMatrix4x2fv}},
    {"3x4", {12, glUniformMatrix3x4fv}},
    {"4x3", {12, glUniformMatrix4x3fv}},
};

namespace lua {

template<typename T>
int SetUniform(
    lua_State* L,
    GLint location,
    int offset,
    std::function<T(lua_State*, int)> getter,
    const GLUniformSetter<T>& setter
) {
    int num = lua_gettop(L) - offset;
    switch (num) {
    case 1: {
        T v0 = getter(L, offset + 1);
        setter.setter1(location, v0);
        break;
    }
    case 2: {
        T v0 = getter(L, offset + 1);
        T v1 = getter(L, offset + 2);
        setter.setter2(location, v0, v1);
        break;
    }
    case 3: {
        T v0 = getter(L, offset + 1);
        T v1 = getter(L, offset + 2);
        T v2 = getter(L, offset + 3);
        setter.setter3(location, v0, v1, v2);
        break;
    }
    case 4: {
        T v0 = getter(L, offset + 1);
        T v1 = getter(L, offset + 2);
        T v2 = getter(L, offset + 3);
        T v3 = getter(L, offset + 4);
        setter.setter4(location, v0, v1, v2, v3);
        break;
    }
    }
    return 0;
}

int SetUniformMatrix(lua_State* L, GLint location, int offset);

} // namespace lua


} // namespace glshaderkit
