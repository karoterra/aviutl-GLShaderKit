#pragma once

#include <memory>
#include <string>
#include <stack>

#include <Windows.h>

#include <glad/gl.h>
#include <glad/wgl.h>

#include "gl_framebuffer.hpp"
#include "gl_vertex.hpp"
#include "gl_shader.hpp"
#include "gl_shader_manager.hpp"
#include "gl_texture.hpp"
#include "release.hpp"
#include "config.hpp"
#include "log.hpp"

namespace glshaderkit {

// OpenGLのコンテキストを管理するクラス
class GLContext {
public:
    // インスタンス取得
    static GLContext& Instance() {
        static GLContext instance;
        return instance;
    }

    // 初期化処理
    void Initialize(const Config& config);

    // 開放処理
    void Release();

    // 初期化済みならtrue
    bool IsInitialized() const {
        return initialized_;
    }

    // レンダリングコンテキストを有効化する
    bool Activate();
    // レンダリングコンテキストを無効化する
    void Deactivate();

    void SetPlaneVertex(int n);
    void SetPointVertex(int n);
    void SetShader(const std::string& path, bool forceReload);
    void Draw(GLenum mode, void* data, int width, int height, int instanceCount);

    GLint GetUniformLocation(const char* name) const;

    void SetTexture2D(int unit, const void* data, int width, int height);

    // GLの情報を取得
    const char* GetString(GLenum name) {
        if (!initialized_) {
            return "";
        }
        const GLubyte* str = glGetString(name);
        if (!str) {
            return "";
        }
        return reinterpret_cast<const char*>(str);
    }

    ReleaseContainer& GetReleaseContainer() {
        return releaseContainer_;
    }

    GLShaderManager& GetShaderManager() {
        return shaderManager_;
    }

private:
    // デフォルトコンストラクタ
    GLContext()
        : initialized_(false), hwnd_(NULL), hdc_(NULL), hglrc_(NULL)
        , shaderManager_(4) {}

    // デストラクタ
    ~GLContext() {}

    // コピーコンストラクタ
    GLContext(const GLContext&) = delete;
    // ムーブコンストラクタ
    GLContext(GLContext&&) = delete;
    // コピー代入
    GLContext& operator=(const GLContext&) = delete;
    // ムーブ代入
    GLContext& operator=(GLContext&&) = delete;

    void ReleaseTextures();

    // 初期化済みならtrue
    bool initialized_;

    HWND hwnd_;
    HDC hdc_;
    HGLRC hglrc_;

    std::unique_ptr<GLFramebuffer> fbo_;
    std::unique_ptr<GLVertex> vertex_;
    GLShaderManager shaderManager_;
    std::stack<GLTexture> textures_;
    ReleaseContainer releaseContainer_;
};

} // namespace glshaderkit
