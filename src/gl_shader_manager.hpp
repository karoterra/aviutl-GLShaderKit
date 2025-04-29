#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <functional>

#include "cache.hpp"
#include "gl_shader.hpp"

namespace glshaderkit {

class GLShaderManager {
public:
    explicit GLShaderManager(size_t capacity);

    void SetShader(const std::string& shaderPath, bool forceReload);

    std::reference_wrapper<GLShader> CreateProgram(const std::string& shaderPath, bool forceReload);

    void CacheActiveShaders();

    const std::optional<std::reference_wrapper<GLShader>>& Current() const {
        return current_;
    }

    void SetCapacity(size_t capacity);

    void Release();

private:
    MoveOnlyLruCache<std::string, GLShader> shaderCache_;
    std::unordered_map<std::string, GLShader> activeShaders_;
    std::optional<std::reference_wrapper<GLShader>> current_;
};

} // namespace glshaderkit
