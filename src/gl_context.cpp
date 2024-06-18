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
    if(!wglMakeCurrent(hdc_, hglrc_)){
        std::string msg = "コンテキストの切替に失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    int glVersion = gladLoaderLoadGL();
    if (glVersion != GLAD_MAKE_VERSION(4, 6)) {
        std::string msg = "GLのロードに失敗しました。" + Log::GetLastErrorMessage();
        throw std::runtime_error(msg);
    }

    wglMakeCurrent(NULL, NULL);

    initialized_ = true;
}

void GLContext::Release() {
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

} // namespace glshaderkit
