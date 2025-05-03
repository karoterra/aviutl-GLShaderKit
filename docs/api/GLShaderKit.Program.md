# GLShaderKit.Program クラス

プログラム（シェーダー）を表すクラス。

deactivate時にプログラムオブジェクトはキャッシュに格納される。
また、deactivateするまではキャッシュ容量を超えるプログラムオブジェクトを作成しても自動的にリソースが破棄されることはない。

## 静的メソッド

### new

```lua
new(shaderPath, forceReload)
```

プログラムのオブジェクトを作成する。

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

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `shaderPath` | `string` | シェーダーのソースファイルの絶対パス |
| `forceReload` | `boolean` | `true` ならシェーダープログラムがキャッシュされていても再コンパイルする。省略した場合は `false` 。 |

**戻り値**

| 型 | 説明 |
| --- | --- |
| `Program` | プログラムインスタンス |

## インスタンスメソッド

### use

```lua
use()
```

プログラムを利用可能にする。

**戻り値**

戻り値はありません。

### setFloat

```lua
setFloat(name, x)
```

`float` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | 値 |

**戻り値**

戻り値はありません。

### setFloat

```lua
setFloat(name, x, y)
```

`vec2` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | x成分 |
| `y` | `number` | y成分 |

**戻り値**

戻り値はありません。

### setFloat

```lua
setFloat(name, x, y, z)
```

`vec3` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `number` | x成分 |
| `y` | `number` | y成分 |
| `z` | `number` | z成分 |

**戻り値**

戻り値はありません。

### setFloat

```lua
setFloat(name, x, y, z, w)
```

`vec4` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

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

### setInt

```lua
setInt(name, x)
```

`int` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | 値 |

**戻り値**

戻り値はありません。

### setInt

```lua
setInt(name, x, y)
```

`ivec2` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |

**戻り値**

戻り値はありません。

### setInt

```lua
setInt(name, x, y, z)
```

`ivec3` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |

**戻り値**

戻り値はありません。

### setInt

```lua
setInt(name, x, y, z, w)
```

`ivec4` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

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

### setUInt

```lua
setUInt(name, x)
```

`uint` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | 値 |

**戻り値**

戻り値はありません。

### setUInt

```lua
setUInt(name, x, y)
```

`uvec2` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |

**戻り値**

戻り値はありません。

### setUInt

```lua
setUInt(name, x, y, z)
```

`uvec3` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `name` | `string` | ユニフォーム変数の名前 |
| `x` | `integer` | x成分 |
| `y` | `integer` | y成分 |
| `z` | `integer` | z成分 |

**戻り値**

戻り値はありません。

### setUInt

```lua
setUInt(name, x, y, z, w)
```

`uvec4` 型のユニフォーム変数を設定する。

`use` の後に実行すること。

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

### setMatrix

```lua
setMatrix(name, matrixType, transpose, value)
```

行列のユニフォーム変数を設定する。

`use` の後に実行すること。

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

