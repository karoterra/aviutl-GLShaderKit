# GLShaderKit.Vertex クラス

頂点配列を表すクラス。

deactivate時にリソースは自動で破棄される。

## 静的メソッド

### new

```lua
new(primitive, n)
```

頂点配列を作成する。

`primitive` に `PLANE` を指定したときは板ポリゴンを表す頂点配列を作成する。
板ポリゴンは横に `n` 分割、縦に `n` 分割される。
$(n + 1) ^ 2$ 個の 頂点が作成される。

| location | 型   | 頂点属性                |
| -------: | ---- | ----------------------- |
|        0 | vec3 | 頂点座標 $[-1, 1]$      |
|        1 | vec2 | テクスチャ座標 $[0, 1]$ |

`primitive` に `POINTS` を指定したときは `n` 個の点群表す頂点配列を作成する。

| location | 型    | 頂点属性                        |
| -------: | ----- | ------------------------------- |
|        0 | float | 頂点番号 $(0, 1, \cdots, n-1 )$ |

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `primitive` | `string` | 頂点のプリミティブ |
| `n` | `integer` |  |

**primitive**

| 値 | 説明 |
| --- | --- |
| `PLANE` | 板ポリゴン |
| `POINTS` | 点群 |

**戻り値**

| 型 | 説明 |
| --- | --- |
| `Vertex` | Vertexインスタンス |

### unbind

```lua
unbind()
```

頂点配列をアンバインドする。

**戻り値**

戻り値はありません。

## インスタンスメソッド

### bind

```lua
bind()
```

頂点配列をバインドする。

**戻り値**

戻り値はありません。

### unbind

```lua
unbind()
```

フレームバッファをアンバインドする。

**戻り値**

戻り値はありません。

### draw

```lua
draw(mode, fbo, instanceCount)
```

描画を実行する。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `mode` | `string` | 描画するプリミティブ |
| `fbo` | `FrameBuffer` | 描画先の `FrameBuffer` インスタンス |
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

### release

```lua
release()
```

リソースを解放する。

**戻り値**

戻り値はありません。

