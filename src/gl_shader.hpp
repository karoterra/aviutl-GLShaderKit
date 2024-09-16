#pragma once

#include <string>
#include <filesystem>

#include <glad/gl.h>

namespace glshaderkit {

class GLShader {
public:
    GLShader(const std::string& path);
    ~GLShader() {
        Release();
    }

    // コピー禁止
    GLShader(const GLShader&) = delete;
    GLShader& operator=(const GLShader&) = delete;

    // ムーブ
    GLShader(GLShader&& other);
    GLShader& operator=(GLShader&& other);

    void Use() const {
        glUseProgram(program_);
    }

    GLint GetUniformLocation(const char* name) const {
        return glGetUniformLocation(program_, name);
    }

private:
    static std::string LoadShaderSource(const std::filesystem::path& basePath);
    static GLuint CompileShader(const char* source, GLenum type);

    void AttachShaderIfExists(const std::filesystem::path& path, GLenum type, const char* defaultSource = nullptr);

    void Release();

    GLuint program_;
};

} // namespace glshaderkit
