cmake_minimum_required(VERSION 3.24)

project(aviutl_GLShaderKit
    LANGUAGES CXX
    VERSION 0.0.0
)

# C++ の設定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lua 5.1.4
set(LUA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/vendor/lua/include)
set(LUA_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/vendor/lua)

# デバッグに使用するAviUtlがあるディレクトリ
set(AVIUTL_DIR ${PROJECT_SOURCE_DIR}/bin/aviutl)

# GLShaderKit.dll
add_library(GLShaderKit SHARED
    src/main.cpp
    src/log.hpp
)
target_include_directories(GLShaderKit
    PRIVATE
        ${LUA_INCLUDE_DIR}
)
target_link_directories(GLShaderKit PRIVATE ${LUA_LIBRARY_DIR})
target_link_libraries(GLShaderKit
    PRIVATE
        lua51
)
target_compile_options(GLShaderKit
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>"
        "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:shift_jis>"
)
target_compile_definitions(GLShaderKit
    PRIVATE
        GL_SHADER_KIT_VERSION="${PROJECT_VERSION}"
)

# 検証用の AviUtl に配置
add_custom_target(debug_deploy ALL
    ${CMAKE_COMMAND} -E make_directory ${AVIUTL_DIR}/script/GLShaderKit
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:GLShaderKit>
        ${PROJECT_SOURCE_DIR}/test/script/GLShaderKit_info.obj
        ${AVIUTL_DIR}/script/GLShaderKit
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS GLShaderKit
    COMMENT "debug_deploy -> ${AVIUTL_DIR}"
)
