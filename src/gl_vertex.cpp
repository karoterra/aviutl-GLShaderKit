#include "gl_vertex.hpp"

#include <vector>

#include "gl_context.hpp"
#include "lua_helper.hpp"

namespace glshaderkit {

GLVertex::GLVertex(int n) : vao_(0), vbo_(0), ibo_(0), primitive_(Primitive::Plane), size_(0), indexCount_(0) {
    if (n < 1) {
        n = 1;
    }
    Initialize(n);
}

GLVertex::GLVertex(Primitive primitive, int n)
    : vao_(0), vbo_(0), ibo_(0)
    , primitive_(primitive), size_(0), indexCount_(0)
{
    switch (primitive) {
    case Primitive::Plane:
        SetPlane(n);
        break;
    case Primitive::Points:
        SetPoints(n);
        break;
    }
}

GLVertex::GLVertex(GLVertex&& other)
    : vao_(other.vao_), vbo_(other.vbo_), ibo_(other.ibo_), size_(other.size_), indexCount_(other.indexCount_) {
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ibo_ = 0;
    other.size_ = 0;
    other.indexCount_ = 0;
}

GLVertex& GLVertex::operator=(GLVertex&& other) {
    if (this == &other) {
        return *this;
    }

    Release();
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
    size_ = other.size_;
    indexCount_ = other.indexCount_;

    other.vao_ = 0;
    other.vbo_ = 0;
    other.ibo_ = 0;
    other.size_ = 0;
    other.indexCount_ = 0;

    return *this;
}

void GLVertex::SetPlane(int n) {
    if (primitive_ == Primitive::Plane && size_ == n) {
        return;
    }

    Release();
    if (n < 1) {
        n = 1;
    }

    // 頂点座標とテクスチャ座標のデータ生成
    int numVertices = (n + 1) * (n + 1);
    std::vector<float> vertices(numVertices * 5);

    int vertexIndex = 0;
    for (int y = 0; y <= n; y++) {
        for (int x = 0; x <= n; x++) {
            float fx = static_cast<float>(x) / n * 2.0f - 1.0f;
            float fy = static_cast<float>(y) / n * 2.0f - 1.0f;
            float u = static_cast<float>(x) / n;
            float v = static_cast<float>(y) / n;
            vertices[vertexIndex++] = fx;
            vertices[vertexIndex++] = fy;
            vertices[vertexIndex++] = 0.0f;
            vertices[vertexIndex++] = u;
            vertices[vertexIndex++] = v;
        }
    }

    // インデックスデータ生成
    int numTriangles = n * n * 2;
    std::vector<GLuint> indices(numTriangles * 3);

    int index = 0;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            int bottomLeft = y * (n + 1) + x;
            int bottomRight = bottomLeft + 1;
            int topLeft = bottomLeft + n + 1;
            int topRight = topLeft + 1;

            indices[index++] = topLeft;
            indices[index++] = bottomLeft;
            indices[index++] = topRight;

            indices[index++] = topRight;
            indices[index++] = bottomLeft;
            indices[index++] = bottomRight;
        }
    }

    // VBO
    glCreateBuffers(1, &vbo_);
    glNamedBufferData(vbo_, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // IBO
    glCreateBuffers(1, &ibo_);
    glNamedBufferData(ibo_, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // VAO
    glCreateVertexArrays(1, &vao_);
    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(vao_, ibo_);

    // 頂点座標
    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_, 0, 0);

    // テクスチャ座標
    glEnableVertexArrayAttrib(vao_, 1);
    glVertexArrayAttribFormat(vao_, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(vao_, 1, 0);

    primitive_ = Primitive::Plane;
    size_ = n;
    indexCount_ = indices.size();
}

void GLVertex::SetPoints(int n) {
    if (primitive_ == Primitive::Points && size_ == n) {
        return;
    }

    Release();
    if (n < 1) {
        n = 1;
    }

    // 頂点番号データ生成
    std::vector<float> vertices(n);
    for (int i = 0; i < n; i++) {
        vertices[i] = static_cast<float>(i);
    }

    // VBO
    glCreateBuffers(1, &vbo_);
    glNamedBufferData(vbo_, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // IBOは不使用

    // VAO
    glCreateVertexArrays(1, &vao_);
    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, 1 * sizeof(float));

    // 頂点番号
    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(vao_, 0, 1, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_, 0, 0);

    primitive_ = Primitive::Points;
    size_ = n;
    indexCount_ = 0;
}

void GLVertex::Initialize(int n) {
    SetPlane(n);
}

void GLVertex::Release() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (ibo_ != 0) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }
    size_ = 0;
    indexCount_ = 0;
}

namespace lua {

void RegisterVertex(lua_State* L) {
    const luaL_Reg staticMethod[] = {
        {"new", VertexNew},
        {"unbind", VertexUnbind},
        {nullptr, nullptr},
    };
    const luaL_Reg metaMethod[] = {
        {"__gc", VertexMetaGC},
        {nullptr, nullptr},
    };
    const luaL_Reg method[] = {
        {"bind", VertexBind},
        {"unbind", VertexUnbind},
        {"draw", VertexDraw},
        {"release", VertexRelease},
        {nullptr, nullptr},
    };

    RegisterLuaClassTable(L, "Vertex", staticMethod);
    RegisterMetaTable(L, kVertexMetaName, metaMethod, method);
}

int VertexNew(lua_State* L) {
    // 第1引数はクラステーブル
    const char* types[] = {"PLANE", "POINTS", nullptr};
    const GLVertex::Primitive primitives[] = {
        GLVertex::Primitive::Plane,
        GLVertex::Primitive::Points,
    };
    auto primitive = primitives[luaL_checkoption(L, 2, "PLANE", types)];
    int n = luaL_checkinteger(L, 3);
    GLVertex* vao = new GLVertex(primitive, n);
    GLContext::Instance().GetReleaseContainer().Add(vao);

    GLVertex** udata = static_cast<GLVertex**>(lua_newuserdata(L, sizeof(GLVertex*)));
    *udata = vao;
    luaL_getmetatable(L, kVertexMetaName);
    lua_setmetatable(L, -2);
    return 1;
}

int VertexMetaGC(lua_State* L) {
    GLVertex* self = GetLuaVertex(L, 1);
    GLContext::Instance().GetReleaseContainer().Remove(self);
    delete self;
    return 0;
}

int VertexBind(lua_State* L) {
    GLVertex* self = GetLuaVertex(L, 1);
    self->Bind();
    return 0;
}

int VertexUnbind(lua_State* L) {
    GLVertex::Unbind();
    return 0;
}

int VertexDraw(lua_State* L) {
    GLVertex* self = GetLuaVertex(L, 1);
    GLenum mode = CheckDrawMode(L, 2);
    GLFramebuffer* fbo = GetLuaFrameBuffer(L, 3);
    int instanceCount = luaL_optinteger(L, 4, 1);

    self->Bind();
    fbo->Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (self->GetPrimitive()) {
    case GLVertex::Primitive::Plane:
        glDrawElementsInstanced(mode, self->IndexCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
        break;
    case GLVertex::Primitive::Points:
        glDrawArraysInstanced(mode, 0, self->Size(), instanceCount);
        break;
    }

    return 0;
}

int VertexRelease(lua_State* L) {
    GLVertex* self = GetLuaVertex(L, 1);
    self->Release();
    return 0;
}

} // namespace lua

} // namespace glshaderkit
