cmake_minimum_required(VERSION 3.24)

project(aviutl_GLShaderKit
    LANGUAGES CXX C
    VERSION 0.4.0
)

enable_testing()

# C++ の設定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GoogleTest
find_package(GTest CONFIG REQUIRED)

# Lua 5.1.4
set(LUA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/vendor/lua/include)
set(LUA_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/vendor/lua)

# glad
set(GLAD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/vendor/glad/include)
set(GLAD_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glad/src)

# デバッグに使用するAviUtlがあるディレクトリ
set(AVIUTL_DIR ${PROJECT_SOURCE_DIR}/bin/aviutl)

# GLShaderKit.dll
add_library(GLShaderKit SHARED
    src/main.cpp
    src/release.cpp
    src/lua_helper.cpp
    src/gl_context.cpp
    src/gl_shader.cpp
    src/gl_shader_manager.cpp
    src/gl_framebuffer.cpp
    src/gl_vertex.cpp
    src/gl_texture.cpp
    src/gl_uniform.cpp
    ${GLAD_SOURCE_DIR}/gl.c
    ${GLAD_SOURCE_DIR}/wgl.c
)
target_include_directories(GLShaderKit
    PRIVATE
        src
        ${LUA_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
)
target_link_directories(GLShaderKit PRIVATE ${LUA_LIBRARY_DIR})
target_link_libraries(GLShaderKit
    PRIVATE
        lua51
        opengl32
)
target_compile_options(GLShaderKit
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>"
        "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:shift_jis>"
)
target_compile_definitions(GLShaderKit
    PRIVATE
        GL_SHADER_KIT_VERSION="${PROJECT_VERSION}"
        NOMINMAX
)

# テスト用実行ファイル
add_executable(GLShaderKit_test
    tests/test_config.cpp
    tests/test_release.cpp
    src/release.cpp
    tests/test_cache.cpp
)
target_include_directories(GLShaderKit_test PUBLIC src tests)
target_link_libraries(GLShaderKit_test
    PRIVATE
        GTest::gmock_main
)
target_compile_options(GLShaderKit_test
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>"
        "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:shift_jis>"
)
target_compile_definitions(GLShaderKit_test
    PRIVATE
        GL_SHADER_KIT_VERSION="${PROJECT_VERSION}"
        NOMINMAX
)

# テストデータ
add_custom_command(TARGET GLShaderKit_test PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/data
        ${CMAKE_BINARY_DIR}/data
)

# 検証用の AviUtl に配置
add_custom_target(debug_deploy ALL
    ${CMAKE_COMMAND} -E make_directory ${AVIUTL_DIR}/script/GLShaderKit
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:GLShaderKit>
        ${PROJECT_SOURCE_DIR}/src/GLShaderKit.ini
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_info.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_draw.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_drawPoints.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_effect.anm
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_effectInstanced.anm
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiShader.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiShader_1.frag
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiShader_2.frag
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture.frag
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture.vert
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture_img0.png
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture_img1.png
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_multiTexture_img2.png
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_uniform.obj
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_uniform.frag
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_uniform.vert
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_gaussianBlur.anm
        ${PROJECT_SOURCE_DIR}/example/GLShaderKit_gaussianBlur.frag
        ${AVIUTL_DIR}/script/GLShaderKit
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS GLShaderKit
    COMMENT "debug_deploy -> ${AVIUTL_DIR}"
)

# リリース用
add_custom_target(package
    COMMAND 7z a publish/${PROJECT_NAME}_v${PROJECT_VERSION}.zip
        $<TARGET_FILE:GLShaderKit>
        ${PROJECT_SOURCE_DIR}/src/GLShaderKit.ini
        README.md
        LICENSE
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS GLShaderKit
    COMMENT "package -> publish/${PROJECT_NAME}_v${PROJECT_VERSION}.zip"
)

# CTest
include(CTest)
include(GoogleTest)
gtest_discover_tests(GLShaderKit_test)
