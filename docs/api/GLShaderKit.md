# GLShaderKit モジュール関数

## version

```lua
version()
```

DLL のバージョンを取得する。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `string` | バージョン文字列 (例: `0.1.0`) |

## isInitialized

```lua
isInitialized()
```

DLL の初期化が完了しているかどうかを取得する。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `boolean` | `true` なら DLL は利用可能、 `false` なら DLL は利用できない。 |

## glVendor

```lua
glVendor()
```

`glGetString(GL_VENDOR)` の結果を返す。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `string` | OpenGL 実装のベンダー |

## glRenderer

```lua
glRenderer()
```

`glGetString(GL_RENDERER)` の結果を返す。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `string` | OpenGL レンダラー |

## glVersion

```lua
glVersion()
```

`glGetString(GL_VERSION)` の結果を返す。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `string` | OpenGL バージョン |

## glslVersion

```lua
glslVersion()
```

`glGetString(GL_SHADING_LANGUAGE_VERSION)` の結果を返す。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `string` | GLSL バージョン |

## activate

```lua
activate()
```

レンダリングコンテキストを有効化する。
スクリプト内でDLLの機能を利用する前に1回実行すること。

**戻り値**

| 型 | 説明 |
| --- | --- |
| `boolean` | `true` なら成功、`false` なら失敗 |

## deactivate

```lua
deactivate()
```

レンダリングコンテキストを無効化する。
スクリプト内でDLLの機能を利用し終わった後に1回実行すること。

**戻り値**

戻り値はありません。

## setPlaneVertex

```lua
setPlaneVertex(n)
```

板ポリゴンを表す頂点配列オブジェクトを利用可能にする。

板ポリゴンは横に n 分割、縦に n 分割される。
$(n + 1) ^ 2$ 個の 頂点が作成される。

| location | 型   | 頂点属性                |
| -------: | ---- | ----------------------- |
|        0 | vec3 | 頂点座標 $[-1, 1]$      |
|        1 | vec2 | テクスチャ座標 $[0, 1]$ |

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `n` | `integer` | 板ポリゴンの分割数。1以上の整数。 |

**戻り値**

戻り値はありません。

## setPointVertex

```lua
setPointVertex(n)
```

n 個の点群を表す頂点配列オブジェクトを利用可能にする。

| location | 型    | 頂点属性                        |
| -------: | ----- | ------------------------------- |
|        0 | float | 頂点番号 $(0, 1, \cdots, n-1 )$ |

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `n` | `integer` | 頂点数。1以上の整数。 |

**戻り値**

戻り値はありません。

## setShader

```lua
setShader(shaderPath, forceReload)
```

シェーダープログラムを利用可能にする。

指定したシェーダーのソースファイルをコンパイル・リンクして利用可能な状態にする。
引数で指定するファイルは1個だけだが、ファイル名が同じで拡張子が対応しているファイルが見つかれば自動で読み込まれる。

| 拡張子  | シェーダー                     |
| ------- | ------------------------------ |
| `.vert` | 頂点シェーダー                 |
| `.frag` | フラグメントシェーダー         |
| `.tesc` | テッセレーション制御シェーダー |
| `.tese` | テッセレーション評価シェーダー |
| `.geom` | ジオメトリシェーダー           |

頂点シェーダーが見つからない場合は以下のデフォルト頂点シェーダーが使用される。
```glsl
#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(iPos, 1.0);
    TexCoord = iTexCoord;
}
```

シェーダープログラムのコンパイル・リンク時にエラーがあった場合はコンソールウィンドウにエラーメッセージが表示される。

コンパイル・リンクされたシェーダープログラムは一定個数キャッシュされる。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `shaderPath` | `string` | シェーダーのソースファイルの絶対パス |
| `forceReload` | `boolean` | `true` ならシェーダープログラムがキャッシュされていても再コンパイルする。省略した場合は `false` 。 |

**戻り値**

戻り値はありません。

## setFloat

```lua
setFloat(name, x)
```

`float` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | 値 |

**戻り値**

戻り値はありません。

## setFloat

```lua
setFloat(name, x, y)
```

`vec2` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | x成分 |
| `y` | `number` | y成分 |

**戻り値**

戻り値はありません。

## setFloat

```lua
setFloat(name, x, y, z)
```

`vec3` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | x成分 |
| `y` | `number` | y成分 |
| `z` | `number` | z成分 |

**戻り値**

戻り値はありません。

## setFloat

```lua
setFloat(name, x, y, z, w)
```

`vec4` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | x成分 |
| `y` | `number` | y成分 |
| `z` | `number` | z成分 |
| `w` | `number` | w成分 |

**戻り値**

戻り値はありません。

## setInt

```lua
setInt(name, x)
```

`int` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | 値 |

**戻り値**

戻り値はありません。

## setInt

```lua
setInt(name, x, y)
```

`ivec2` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |

**戻り値**

戻り値はありません。

## setInt

```lua
setInt(name, x, y, z)
```

`ivec3` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |

**戻り値**

戻り値はありません。

## setInt

```lua
setInt(name, x, y, z, w)
```

`ivec4` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |
| `w` | `integer` | w成分 |

**戻り値**

戻り値はありません。

## setUInt

```lua
setUInt(name, x)
```

`uint` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | 値 |

**戻り値**

戻り値はありません。

## setUInt

```lua
setUInt(name, x, y)
```

`uvec2` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |

**戻り値**

戻り値はありません。

## setUInt

```lua
setUInt(name, x, y, z)
```

`uvec3` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |

**戻り値**

戻り値はありません。

## setUInt

```lua
setUInt(name, x, y, z, w)
```

`uvec4` 型のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |
| `w` | `integer` | w成分 |

**戻り値**

戻り値はありません。

## setMatrix

```lua
setMatrix(name, matrixType, transpose, value)
```

行列のユニフォーム変数を設定する。

`setShader` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `matrixType` | `string` | 行列の形 |
| `transpose` | `boolean` | `true` なら転置する |
| `value` | `number[]` | 行列の各要素の値。要素数は行列の要素数とあわせること |

**matrixType**

| 値 |
| --- |
| `2x2` |
| `3x3` |
| `4x4` |
| `2x3` |
| `3x2` |
| `2x4` |
| `4x2` |
| `3x4` |
| `4x3` |

**戻り値**

戻り値はありません。

## setTexture2D

```lua
setTexture2D(unit, data, width, height)
```

テクスチャをバインドする。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `unit` | `integer` | テクスチャのユニット番号 |
| `data` | `lightuserdata` | テクスチャのBGRA配列へのポインタ |
| `width` | `integer` | テクスチャの幅 |
| `height` | `integer` | テクスチャの高さ |

**戻り値**

戻り値はありません。

## draw

```lua
draw(mode, data, width, height, instanceCount)
```

描画する。

描画後にテクスチャは破棄される。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `mode` | `string` | 描画するプリミティブ |
| `data` | `lightuserdata` | 描画先のBGRA配列へのポインタ |
| `width` | `integer` | 描画先の幅 |
| `height` | `integer` | 描画先の高さ |
| `instanceCount` | `integer` | 描画インスタンス数。1以上の整数を指定する。省略した場合は 1 。 |

**mode**

| 値 |
| --- |
| `POINTS` |
| `LINE_STRIP` |
| `LINE_LOOP` |
| `LINES` |
| `LINE_STRIP_ADJACENCY` |
| `LINES_ADJACENCY` |
| `TRIANGLE_STRIP` |
| `TRIANGLE_FAN` |
| `TRIANGLES` |
| `TRIANGLE_STRIP_ADJACENCY` |
| `TRIANGLES_ADJACENCY` |
| `PATCHES` |

**戻り値**

戻り値はありません。

