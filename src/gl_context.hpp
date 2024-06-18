#pragma once

#include <string>

#include <Windows.h>

#include <glad/gl.h>
#include <glad/wgl.h>

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

    // 初期化済みならtrue
    bool initialized_;

    HWND hwnd_;
    HDC hdc_;
    HGLRC hglrc_;
};

} // namespace glshaderkit
