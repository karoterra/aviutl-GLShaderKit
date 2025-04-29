#include "gl_context.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <format>

#include "log.hpp"

namespace glshaderkit {

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    return DefWindowProcA(hwnd, msg, wp, lp);
}

#ifdef _DEBUG
void GLAPIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::string sourceStr;
    switch (source) {
    case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW_SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER_COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD_PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
    default: sourceStr = "unknown";
    }

    std::string typeStr;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "MARKER"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: typeStr = "PUSH_GROUP"; break;
    case GL_DEBUG_TYPE_POP_GROUP: typeStr = "POP_GROUP"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
    default: typeStr = "unknown";
    }

    std::string severityStr;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: severityStr = "HIGH"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW: severityStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    default: severityStr = "unknown";
    }

    std::cout << std::format("[GLShaderKit][GL]source: {}, type: {}, id: {}, severity: {}, message: {}", sourceStr, typeStr, id, severityStr, message) << std::endl;
}
#endif

void GLContext::Initialize(const Config& config) {
    if (initialized_) {
        return;
    }

    // OpenGL を使用するためのウィンドウを作成
    WNDCLASS wc = {
        .style = CS_OWNDC,
        .lpfnWndProc = WndProc,
        .hInstance = GetModuleHandleA("GLShaderKit.auf"),
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
#ifdef _DEBUG
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
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

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugOutputCallback, nullptr);
#endif

    fbo_.reset(new GLFramebuffer(100, 100));
    vertex_.reset(new GLVertex(1));
    shaderManager_.SetCapacity(config.shaderCacheCapacity);

    glEnable(GL_PROGRAM_POINT_SIZE);

    wglMakeCurrent(NULL, NULL);

    initialized_ = true;
}

void GLContext::Release() {
    Activate();

    // GLオブジェクトの開放
    fbo_.reset();
    vertex_.reset();
    shaderManager_.Release();
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

bool GLContext::Activate() {
    if (!initialized_) {
        return false;
    }
    return wglMakeCurrent(hdc_, hglrc_);
}

void GLContext::Deactivate() {
    releaseContainer_.ReleaseAll();
    shaderManager_.CacheActiveShaders();
    wglMakeCurrent(NULL, NULL);
}

void GLContext::SetPlaneVertex(int n) {
    vertex_->SetPlane(n);
}

void GLContext::SetPointVertex(int n) {
    vertex_->SetPoints(n);
}

void GLContext::SetShader(const std::string& path, bool forceReload) {
    if (path.empty()) {
        return;
    }

    try {
        shaderManager_.SetShader(path, forceReload);
        shaderManager_.Current()->get().Use();
    }
    catch (const std::runtime_error& e) {
        Log::Error(e.what());
    }
}

void GLContext::Draw(GLenum mode, void* data, int width, int height, int instanceCount) {
    if (!shaderManager_.Current()) {
        return;
    }

    if (instanceCount < 1) {
        instanceCount = 1;
    }

    fbo_->Resize(width, height);
    fbo_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vertex_->Bind();

    switch (vertex_->GetPrimitive()) {
    case GLVertex::Primitive::Plane:
        glDrawElementsInstanced(mode, vertex_->IndexCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
        break;
    case GLVertex::Primitive::Points:
        glDrawArraysInstanced(mode, 0, vertex_->Size(), instanceCount);
        break;
    }
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);

    GLTexture::Unbind();
    ReleaseTextures();
    GLVertex::Unbind();
    GLFramebuffer::Unbind();
}

GLint GLContext::GetUniformLocation(const char* name) const {
    auto current = shaderManager_.Current();
    if (current) {
        return current->get().GetUniformLocation(name);
    }
    else {
        return -1;
    }
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
