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
#include "gl_texture.hpp"
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
    void Initialize();

    // 開放処理
    void Release();

    // 初期化済みならtrue
    bool IsInitialized() const {
        return initialized_;
    }

    // レンダリングコンテキストを有効化する
    bool Activate() {
        if (!initialized_) {
            return false;
        }
        return wglMakeCurrent(hdc_, hglrc_);
    }

    // レンダリングコンテキストを無効化する
    void Deactivate() {
        wglMakeCurrent(NULL, NULL);
    }

    void SetVertex(int n);
    void SetShader(const std::string& path);
    void Draw(void* data, int width, int height);

    void SetFloat(const char* name, float v0);
    void SetVec2(const char* name, float v0, float v1);
    void SetVec3(const char* name, float v0, float v1, float v2);
    void SetVec4(const char* name, float v0, float v1, float v2, float v3);

    void SetInt(const char* name, int v0);
    void SetIVec2(const char* name, int v0, int v1);
    void SetIVec3(const char* name, int v0, int v1, int v2);
    void SetIVec4(const char* name, int v0, int v1, int v2, int v3);

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

private:
    // デフォルトコンストラクタ
    GLContext() : initialized_(false), hwnd_(NULL), hdc_(NULL), hglrc_(NULL) {}

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
    std::unique_ptr<GLShader> shader_;
    std::string currentShader_;
    std::stack<GLTexture> textures_;
};

} // namespace glshaderkit
