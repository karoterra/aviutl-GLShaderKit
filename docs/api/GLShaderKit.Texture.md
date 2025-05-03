# GLShaderKit.Texture クラス

テクスチャを表すクラス。

deactivate時にリソースは自動で破棄される。

## 静的メソッド

### new

```lua
new(data, width, height)
```

テクスチャを作成する。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `data` | `lightuserdata` | テクスチャのBGRA配列へのポインタ |
| `width` | `integer` | テクスチャの幅 |
| `height` | `integer` | テクスチャの高さ |

**戻り値**

| 型 | 説明 |
| --- | --- |
| `Texture` | Textureインスタンス |

### unbind

```lua
unbind()
```

テクスチャをアンバインドする。

**戻り値**

戻り値はありません。

## インスタンスメソッド

### bind

```lua
bind(unit)
```

テクスチャをバインドする。

**引数**

| 名前 | 型 | 説明 |
| --- | --- | --- |
| `unit` | `integer` | テクスチャのユニット番号 |

**戻り値**

戻り値はありません。

### unbind

```lua
unbind()
```

テクスチャをアンバインドする。

**戻り値**

戻り値はありません。

### release

```lua
release()
```

リソースを解放する。

**戻り値**

戻り値はありません。

