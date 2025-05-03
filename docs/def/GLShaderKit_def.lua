---@meta GLShaderKit

local GLShaderKit = {}

---DLL のバージョンを取得する。
---@return string # バージョン文字列 (例: `0.1.0`)
function GLShaderKit.version() end

---DLL の初期化が完了しているかどうかを取得する。
---@return boolean # `true` なら DLL は利用可能、 `false` なら DLL は利用できない。
function GLShaderKit.isInitialized() end

---`glGetString(GL_VENDOR)` の結果を返す。
---@return string # OpenGL 実装のベンダー
function GLShaderKit.glVendor() end

---`glGetString(GL_RENDERER)` の結果を返す。
---@return string # OpenGL レンダラー
function GLShaderKit.glRenderer() end

---`glGetString(GL_VERSION)` の結果を返す。
---@return string # OpenGL バージョン
function GLShaderKit.glVersion() end

---`glGetString(GL_SHADING_LANGUAGE_VERSION)` の結果を返す。
---@return string # GLSL バージョン
function GLShaderKit.glslVersion() end

---レンダリングコンテキストを有効化する。
---スクリプト内でDLLの機能を利用する前に1回実行すること。
---@return boolean # `true` なら成功、`false` なら失敗
function GLShaderKit.activate() end

---レンダリングコンテキストを無効化する。
---スクリプト内でDLLの機能を利用し終わった後に1回実行すること。
function GLShaderKit.deactivate() end

---板ポリゴンを表す頂点配列オブジェクトを利用可能にする。
---
---板ポリゴンは横に n 分割、縦に n 分割される。
---$(n + 1) ^ 2$ 個の 頂点が作成される。
---
---| location | 型   | 頂点属性                |
---| -------: | ---- | ----------------------- |
---|        0 | vec3 | 頂点座標 $[-1, 1]$      |
---|        1 | vec2 | テクスチャ座標 $[0, 1]$ |
---@param n integer 板ポリゴンの分割数。1以上の整数。
function GLShaderKit.setPlaneVertex(n) end

---n 個の点群を表す頂点配列オブジェクトを利用可能にする。
---
---| location | 型    | 頂点属性                        |
---| -------: | ----- | ------------------------------- |
---|        0 | float | 頂点番号 $(0, 1, \cdots, n-1 )$ |
---@param n integer 頂点数。1以上の整数。
function GLShaderKit.setPointVertex(n) end

---シェーダープログラムを利用可能にする。
---
---指定したシェーダーのソースファイルをコンパイル・リンクして利用可能な状態にする。
---引数で指定するファイルは1個だけだが、ファイル名が同じで拡張子が対応しているファイルが見つかれば自動で読み込まれる。
---
---| 拡張子  | シェーダー                     |
---| ------- | ------------------------------ |
---| `.vert` | 頂点シェーダー                 |
---| `.frag` | フラグメントシェーダー         |
---| `.tesc` | テッセレーション制御シェーダー |
---| `.tese` | テッセレーション評価シェーダー |
---| `.geom` | ジオメトリシェーダー           |
---
---頂点シェーダーが見つからない場合は以下のデフォルト頂点シェーダーが使用される。
---```glsl
---#version 460 core
---
---layout(location = 0) in vec3 iPos;
---layout(location = 1) in vec2 iTexCoord;
---
---out vec2 TexCoord;
---
---void main() {
---    gl_Position = vec4(iPos, 1.0);
---    TexCoord = iTexCoord;
---}
---```
---
---シェーダープログラムのコンパイル・リンク時にエラーがあった場合はコンソールウィンドウにエラーメッセージが表示される。
---
---コンパイル・リンクされたシェーダープログラムは一定個数キャッシュされる。
---@param shaderPath string シェーダーのソースファイルの絶対パス
---@param forceReload? boolean `true` ならシェーダープログラムがキャッシュされていても再コンパイルする。省略した場合は `false` 。
function GLShaderKit.setShader(shaderPath, forceReload) end

---`float` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number 値
function GLShaderKit.setFloat(name, x) end

---`vec2` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
function GLShaderKit.setFloat(name, x, y) end

---`vec3` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
---@param z number z成分
function GLShaderKit.setFloat(name, x, y, z) end

---`vec4` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
---@param z number z成分
---@param w number w成分
function GLShaderKit.setFloat(name, x, y, z, w) end

---`int` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer 値
function GLShaderKit.setInt(name, x) end

---`ivec2` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
function GLShaderKit.setInt(name, x, y) end

---`ivec3` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
function GLShaderKit.setInt(name, x, y, z) end

---`ivec4` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
---@param w integer w成分
function GLShaderKit.setInt(name, x, y, z, w) end

---`uint` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer 値
function GLShaderKit.setUInt(name, x) end

---`uvec2` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
function GLShaderKit.setUInt(name, x, y) end

---`uvec3` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
function GLShaderKit.setUInt(name, x, y, z) end

---`uvec4` 型のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
---@param w integer w成分
function GLShaderKit.setUInt(name, x, y, z, w) end

---行列のユニフォーム変数を設定する。
---
---`setShader` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param matrixType string 行列の形
---| '"2x2"'
---| '"3x3"'
---| '"4x4"'
---| '"2x3"'
---| '"3x2"'
---| '"2x4"'
---| '"4x2"'
---| '"3x4"'
---| '"4x3"'
---@param transpose boolean `true` なら転置する
---@param value number[] 行列の各要素の値。要素数は行列の要素数とあわせること
function GLShaderKit.setMatrix(name, matrixType, transpose, value) end

---テクスチャをバインドする。
---@param unit integer テクスチャのユニット番号
---@param data lightuserdata テクスチャのBGRA配列へのポインタ
---@param width integer テクスチャの幅
---@param height integer テクスチャの高さ
function GLShaderKit.setTexture2D(unit, data, width, height) end

---描画する。
---
---描画後にテクスチャは破棄される。
---@param mode string 描画するプリミティブ
---| '"POINTS"'
---| '"LINE_STRIP"'
---| '"LINE_LOOP"'
---| '"LINES"'
---| '"LINE_STRIP_ADJACENCY"'
---| '"LINES_ADJACENCY"'
---| '"TRIANGLE_STRIP"'
---| '"TRIANGLE_FAN"'
---| '"TRIANGLES"'
---| '"TRIANGLE_STRIP_ADJACENCY"'
---| '"TRIANGLES_ADJACENCY"'
---| '"PATCHES"'
---@param data lightuserdata 描画先のBGRA配列へのポインタ
---@param width integer 描画先の幅
---@param height integer 描画先の高さ
---@param instanceCount? integer 描画インスタンス数。1以上の整数を指定する。省略した場合は 1 。
function GLShaderKit.draw(mode, data, width, height, instanceCount) end

GLShaderKit.FrameBuffer = {}

---@class FrameBuffer
local FrameBuffer = {}

---フレームバッファを作成する。
---@param width integer フレームバッファの幅
---@param height integer フレームバッファの高さ
function GLShaderKit.FrameBuffer:new(width, height)
    ---@type FrameBuffer
    local o = {
        bind = FrameBuffer.bind,
        unbind = FrameBuffer.unbind,
        bindTexture = FrameBuffer.bindTexture,
        readPixels = FrameBuffer.readPixels,
        release = FrameBuffer.release,
    }
    return o
end

---フレームバッファをアンバインドする。
function GLShaderKit.FrameBuffer:unbind() end

---フレームバッファをバインドする。
function FrameBuffer:bind() end

---フレームバッファをアンバインドする。
function FrameBuffer:unbind() end

---フレームバッファのカラーバッファをテクスチャとしてバインドする。
---@param unit integer テクスチャのユニット番号
function FrameBuffer:bindTexture(unit) end

---フレームバッファのカラーバッファの画像データを取得する。
---@param data lightuserdata BGRA配列へのポインタ
function FrameBuffer:readPixels(data) end

---リソースを解放する。
function FrameBuffer:release() end

GLShaderKit.Program = {}

---@class Program
local Program = {}

---プログラムのオブジェクトを作成する。
---
---指定したシェーダーのソースファイルをコンパイル・リンクして利用可能な状態にする。
---引数で指定するファイルは1個だけだが、ファイル名が同じで拡張子が対応しているファイルが見つかれば自動で読み込まれる。
---
---| 拡張子  | シェーダー                     |
---| ------- | ------------------------------ |
---| `.vert` | 頂点シェーダー                 |
---| `.frag` | フラグメントシェーダー         |
---| `.tesc` | テッセレーション制御シェーダー |
---| `.tese` | テッセレーション評価シェーダー |
---| `.geom` | ジオメトリシェーダー           |
---
---頂点シェーダーが見つからない場合は以下のデフォルト頂点シェーダーが使用される。
---```glsl
---#version 460 core
---
---layout(location = 0) in vec3 iPos;
---layout(location = 1) in vec2 iTexCoord;
---
---out vec2 TexCoord;
---
---void main() {
---    gl_Position = vec4(iPos, 1.0);
---    TexCoord = iTexCoord;
---}
---```
---
---シェーダープログラムのコンパイル・リンク時にエラーがあった場合はコンソールウィンドウにエラーメッセージが表示される。
---@param shaderPath string シェーダーのソースファイルの絶対パス
---@param forceReload? boolean `true` ならシェーダープログラムがキャッシュされていても再コンパイルする。省略した場合は `false` 。
function GLShaderKit.Program:new(shaderPath, forceReload)
    ---@type Program
    local o = {
        use = Program.use,
        setFloat = Program.setFloat,
        setInt = Program.setInt,
        setUInt = Program.setUInt,
        setMatrix = Program.setMatrix,
    }
    return o
end

---プログラムを利用可能にする。
function Program:use() end

---`float` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number 値
function Program:setFloat(name, x) end

---`vec2` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
function Program:setFloat(name, x, y) end

---`vec3` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
---@param z number z成分
function Program:setFloat(name, x, y, z) end

---`vec4` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x number x成分
---@param y number y成分
---@param z number z成分
---@param w number w成分
function Program:setFloat(name, x, y, z, w) end

---`int` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer 値
function Program:setInt(name, x) end

---`ivec2` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
function Program:setInt(name, x, y) end

---`ivec3` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
function Program:setInt(name, x, y, z) end

---`ivec4` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
---@param w integer w成分
function Program:setInt(name, x, y, z, w) end

---`uint` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer 値
function Program:setUInt(name, x) end

---`uvec2` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
function Program:setUInt(name, x, y) end

---`uvec3` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
function Program:setUInt(name, x, y, z) end

---`uvec4` 型のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param x integer x成分
---@param y integer y成分
---@param z integer z成分
---@param w integer w成分
function Program:setUInt(name, x, y, z, w) end

---行列のユニフォーム変数を設定する。
---
---`use` の後に実行すること。
---@param name string ユニフォーム変数の名前
---@param matrixType string 行列の形
---| '"2x2"'
---| '"3x3"'
---| '"4x4"'
---| '"2x3"'
---| '"3x2"'
---| '"2x4"'
---| '"4x2"'
---| '"3x4"'
---| '"4x3"'
---@param transpose boolean `true` なら転置する
---@param value number[] 行列の各要素の値。要素数は行列の要素数とあわせること
function Program:setMatrix(name, matrixType, transpose, value) end

GLShaderKit.Texture = {}

---@class Texture
local Texture = {}

---テクスチャを作成する。
---@param data lightuserdata テクスチャのBGRA配列へのポインタ
---@param width integer テクスチャの幅
---@param height integer テクスチャの高さ
function GLShaderKit.Texture:new(data, width, height)
    ---@type Texture
    local o = {
        bind = Texture.bind,
        unbind = Texture.unbind,
        release = Texture.release,
    }
    return o
end

---テクスチャをアンバインドする。
function GLShaderKit.Texture:unbind() end

---テクスチャをバインドする。
---@param unit integer テクスチャのユニット番号
function Texture:bind(unit) end

---テクスチャをアンバインドする。
function Texture:unbind() end

---リソースを解放する。
function Texture:release() end

GLShaderKit.Vertex = {}

---@class Vertex
local Vertex = {}

---頂点配列を作成する。
---
---`primitive` に `PLANE` を指定したときは板ポリゴンを表す頂点配列を作成する。
---板ポリゴンは横に `n` 分割、縦に `n` 分割される。
---$(n + 1) ^ 2$ 個の 頂点が作成される。
---
---| location | 型   | 頂点属性                |
---| -------: | ---- | ----------------------- |
---|        0 | vec3 | 頂点座標 $[-1, 1]$      |
---|        1 | vec2 | テクスチャ座標 $[0, 1]$ |
---
---`primitive` に `POINTS` を指定したときは `n` 個の点群表す頂点配列を作成する。
---
---| location | 型    | 頂点属性                        |
---| -------: | ----- | ------------------------------- |
---|        0 | float | 頂点番号 $(0, 1, \cdots, n-1 )$ |
---@param primitive string 頂点のプリミティブ
---| '"PLANE"' # 板ポリゴン
---| '"POINTS"' # 点群
---@param n integer 
function GLShaderKit.Vertex:new(primitive, n)
    ---@type Vertex
    local o = {
        bind = Vertex.bind,
        unbind = Vertex.unbind,
        draw = Vertex.draw,
        release = Vertex.release,
    }
    return o
end

---頂点配列をアンバインドする。
function GLShaderKit.Vertex:unbind() end

---頂点配列をバインドする。
function Vertex:bind() end

---フレームバッファをアンバインドする。
function Vertex:unbind() end

---描画を実行する。
---@param mode string 描画するプリミティブ
---| '"POINTS"'
---| '"LINE_STRIP"'
---| '"LINE_LOOP"'
---| '"LINES"'
---| '"LINE_STRIP_ADJACENCY"'
---| '"LINES_ADJACENCY"'
---| '"TRIANGLE_STRIP"'
---| '"TRIANGLE_FAN"'
---| '"TRIANGLES"'
---| '"TRIANGLE_STRIP_ADJACENCY"'
---| '"TRIANGLES_ADJACENCY"'
---| '"PATCHES"'
---@param fbo FrameBuffer 描画先の `FrameBuffer` インスタンス
---@param instanceCount? integer 描画インスタンス数。1以上の整数を指定する。省略した場合は 1 。
function Vertex:draw(mode, fbo, instanceCount) end

---リソースを解放する。
function Vertex:release() end

return GLShaderKit
