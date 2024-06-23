#include "gl_context.hpp"

#include <string>
#include <stdexcept>

#include "log.hpp"

namespace glshaderkit {

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    return DefWindowProcA(hwnd, msg, wp, lp);
}

void GLContext::Initialize() {
    if (initialized_) {
        return;
    }

    // OpenGL を使用するためのウィンドウを作成
    WNDCLASS wc = {
        .style = CS_OWNDC,
        .lpfnWndProc = WndProc,
        .hInstance = GetModuleHandleA(nullptr),
        .lpszClassName = "DummyGLShaderKitWindow",
    };
    if (!RegisterClassA(&wc)) {
        std::string msg = "ウィンドウクラスの登録に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }
    hwnd_ = CreateWindowExA(
        0,
        wc.lpszClassName,
        "Dummy GLShaderKit Window",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, wc.hInstance, nullptr
    );
    if (hwnd_ == NULL) {
        std::string msg = "ウィンドウの作成に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }
    hdc_ = GetDC(hwnd_);
    if (hdc_ == NULL) {
        throw std::runtime_error("デバイスコンテキストの取得に失敗しました。");
    }

    // OpenGL の初期化
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 16,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
    };
    int pixelFormat = ChoosePixelFormat(hdc_, &pfd);
    if (pixelFormat == 0) {
        std::string msg = "ChoosePixelFormatに失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }
    if (!SetPixelFormat(hdc_, pixelFormat, &pfd)) {
        std::string msg = "SetPixelFormatに失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    wglMakeCurrent(NULL, NULL);

    HGLRC tempContext = wglCreateContext(hdc_);
    if (tempContext == NULL) {
        std::string msg = "仮コンテキストの作成に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }
    if (!wglMakeCurrent(hdc_, tempContext)) {
        std::string msg = "仮コンテキストへの切替に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }
    int wglVersion = gladLoaderLoadWGL(hdc_);
    if (wglVersion == 0) {
        std::string msg = "WGLのロードに失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        0
    };
    hglrc_ = wglCreateContextAttribsARB(hdc_, NULL, attribs);
    if (hglrc_ == NULL) {
        std::string msg = "コンテキストの作成に失敗しました。デバイスがOpenGL 4.6に対応しているか確認してください。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);
    if (!wglMakeCurrent(hdc_, hglrc_)) {
        std::string msg = "コンテキストの切替に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    int glVersion = gladLoaderLoadGL();
    if (glVersion != GLAD_MAKE_VERSION(4, 6)) {
        std::string msg = "GLのロードに失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    fbo_.reset(new GLFramebuffer(100, 100));
    vertex_.reset(new GLVertex(1));

    wglMakeCurrent(NULL, NULL);

    initialized_ = true;
}

void GLContext::Release() {
    Activate();

    // GLオブジェクトの開放
    fbo_.reset();
    shader_.reset();
    vertex_.reset();
    ReleaseTextures();

    Deactivate();

    if (hglrc_ != NULL) {
        wglDeleteContext(hglrc_);
        hglrc_ = NULL;
    }
    if (hdc_ != NULL) {
        ReleaseDC(hwnd_, hdc_);
        hdc_ = NULL;
    }
    if (hwnd_ != NULL) {
        DestroyWindow(hwnd_);
        hwnd_ = NULL;
    }

    initialized_ = false;
}

void GLContext::SetVertex(int n) {
    vertex_->Resize(n);
}

void GLContext::SetShader(const std::string& path) {
    if (path.empty() || path == currentShader_) {
        return;
    }

    try {
        shader_.reset(new GLShader(path));
        shader_->Use();
        currentShader_ = path;
    }
    catch (const std::runtime_error& e) {
        Log::Error(e.what());
        shader_.reset();
        currentShader_ = "";
    }
}

void GLContext::Draw(void* data, int width, int height) {
    if (!shader_) {
        return;
    }

    fbo_->Resize(width, height);
    fbo_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vertex_->Bind();

    glDrawElements(GL_TRIANGLES, vertex_->IndexCount(), GL_UNSIGNED_INT, nullptr);
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);

    GLTexture::Unbind();
    ReleaseTextures();
    GLVertex::Unbind();
    GLFramebuffer::Unbind();
}

void GLContext::SetFloat(const char* name, float v0) {
    if (shader_) glUniform1f(shader_->GetUniformLocation(name), v0);
}

void GLContext::SetVec2(const char* name, float v0, float v1) {
    if (shader_) glUniform2f(shader_->GetUniformLocation(name), v0, v1);
}

void GLContext::SetVec3(const char* name, float v0, float v1, float v2) {
    if (shader_) glUniform3f(shader_->GetUniformLocation(name), v0, v1, v2);
}

void GLContext::SetVec4(const char* name, float v0, float v1, float v2, float v3) {
    if (shader_) glUniform4f(shader_->GetUniformLocation(name), v0, v1, v2, v3);
}

void GLContext::SetInt(const char* name, int v0) {
    if (shader_) glUniform1i(shader_->GetUniformLocation(name), v0);
}

void GLContext::SetIVec2(const char* name, int v0, int v1) {
    if (shader_) glUniform2i(shader_->GetUniformLocation(name), v0, v1);
}

void GLContext::SetIVec3(const char* name, int v0, int v1, int v2) {
    if (shader_) glUniform3i(shader_->GetUniformLocation(name), v0, v1, v2);
}

void GLContext::SetIVec4(const char* name, int v0, int v1, int v2, int v3) {
    if (shader_) glUniform4i(shader_->GetUniformLocation(name), v0, v1, v2, v3);
}

void GLContext::SetTexture2D(int unit, const void* data, int width, int height) {
    textures_.emplace(data, width, height);
    textures_.top().Bind(unit);
}

void GLContext::ReleaseTextures() {
    while (!textures_.empty()) {
        textures_.pop();
    }
}

} // namespace glshaderkit
