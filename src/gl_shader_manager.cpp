#include "gl_shader_manager.hpp"

namespace glshaderkit {

GLShaderManager::GLShaderManager(size_t capacity)
    : shaderCache_(capacity)
    , current_(std::nullopt)
{}

void GLShaderManager::SetShader(const std::string& shaderPath, bool forceReload) {
    // shaderCache_ から探す
    auto cached = shaderCache_.Get(shaderPath);
    if (cached) {
        if (forceReload) {
            shaderCache_.Put(shaderPath, GLShader(shaderPath));
            current_ = shaderCache_.Get(shaderPath);
        } else {
            current_ = cached;
        }
        return;
    }

    // activeShaders_ から探す
    auto activeIt = activeShaders_.find(shaderPath);
    if (activeIt != activeShaders_.end()) {
        if (forceReload) {
            activeIt->second = GLShader(shaderPath);
        }
        current_ = std::ref(activeIt->second);
        return;
    }

    // どこにもキャッシュされていない場合
    shaderCache_.Put(shaderPath, GLShader(shaderPath));
    current_ = shaderCache_.Get(shaderPath);
}

std::reference_wrapper<GLShader> GLShaderManager::CreateProgram(
    const std::string& shaderPath,
    bool forceReload
) {
    // activeShaders_ から探す
    auto activeIt = activeShaders_.find(shaderPath);
    if (activeIt != activeShaders_.end()) {
        if (forceReload) {
            activeIt->second = GLShader(shaderPath);
        }
        return std::ref(activeIt->second);
    }

    // shaderCache_ から探す
    auto cached = shaderCache_.Get(shaderPath);
    if (cached) {
        GLShader shader = forceReload ? GLShader(shaderPath) : std::move(cached->get());
        auto [it, success] = activeShaders_.emplace(shaderPath, std::move(shader));
        shaderCache_.Erase(shaderPath);
        return std::ref(it->second);
    }

    auto [it, success] = activeShaders_.emplace(shaderPath, GLShader(shaderPath));
    return std::ref(it->second);
}

void GLShaderManager::CacheActiveShaders() {
    for (auto& [path, shader] : activeShaders_) {
        shaderCache_.Put(path, std::move(shader));
    }
    activeShaders_.clear();
}

void GLShaderManager::SetCapacity(size_t capacity) {
    shaderCache_.SetCapacity(capacity);
}

void GLShaderManager::Release() {
    current_ = std::nullopt;
    activeShaders_.clear();
    shaderCache_.Clear();
}

} // namespace glshaderkit
