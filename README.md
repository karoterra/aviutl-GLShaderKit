# GLShaderKit

[AviUtl](http://spring-fragrance.mints.ne.jp/aviutl/)
拡張編集スクリプトで GLSL を使用するための DLL です。

## 動作環境

お使いの PC が OpenGL 4.6 に対応している必要があります。

最新の patch.aul の導入を推奨します。

## 導入方法

1. Microsoft Visual C++ 再頒布可能パッケージがインストールされている必要があります。お使いの PC にまだインストールされていない場合はあらかじめインストールしてください。
   - アーキテクチャは x86 用のものが必要です。
   - バージョンは Visual Studio 2022 に対応したものが必要です。
     - 例えば Microsoft Visual C++ 2015-2022 再頒布可能パッケージがインストールされていれば問題ありません。
   - インストーラのダウンロードやインストール方法については以下を参照してください。
     - [サポートされている最新の Visual C++ 再頒布可能パッケージのダウンロード | Microsoft Learn](https://learn.microsoft.com/ja-JP/cpp/windows/latest-supported-vc-redist?view=msvc-170)
     - [Visual C++ 再頒布可能パッケージ - /AviUtl](https://scrapbox.io/aviutl/Visual_C++_%E5%86%8D%E9%A0%92%E5%B8%83%E5%8F%AF%E8%83%BD%E3%83%91%E3%83%83%E3%82%B1%E3%83%BC%E3%82%B8)
2. [Releases](https://github.com/karoterra/aviutl-GLShaderKit/releases/)
   から最新版の ZIP ファイルをダウンロードしてください。
3. ZIP ファイルを展開し、以下のファイルを適切な場所に配置してください。
   - `GLShaderKit.dll`
   - `GLShaderKit.ini`

## 使用例

GLShaderKit を使用したスクリプト・シェーダーのサンプルは [example](example) を参照してください。

## 設定

`GLShaderKit.dll` と同じフォルダに置いた `GLShaderKit.ini` は設定ファイルです。

### 設定項目
| 項目名                | 説明                               |
| --------------------- | ---------------------------------- |
| `shaderCacheCapacity` | シェーダープログラムのキャッシュ数 |

## 関数

DLL で提供される関数は以下の通りです。

### `version()`
DLL のバージョンを取得する。

#### 戻り値
| 型     | 説明                           |
| ------ | ------------------------------ |
| string | DLLのバージョン（例: `0.1.0`） |

### `isInitialized()`
DLL の初期化が完了しているかどうかを取得する。

#### 戻り値
| 型      | 説明                                                        |
| ------- | ----------------------------------------------------------- |
| boolean | `true` なら DLL は利用可能、`false` なら DLL は利用できない |

### `glVendor()`
`glGetString(GL_VENDOR)` の結果を返す。

#### 戻り値
| 型     | 説明                  |
| ------ | --------------------- |
| string | OpenGL 実装のベンダー |

### `glRenderer()`
`glGetString(GL_RENDERER)` の結果を返す。

#### 戻り値
| 型     | 説明              |
| ------ | ----------------- |
| string | OpenGL レンダラー |

### `glVersion()`
`glGetString(GL_VERSION)` の結果を返す。

#### 戻り値
| 型     | 説明              |
| ------ | ----------------- |
| string | OpenGL バージョン |

### `glslVersion()`
`glGetString(GL_SHADING_LANGUAGE_VERSION)` の結果を返す。

#### 戻り値
| 型     | 説明            |
| ------ | --------------- |
| string | GLSL バージョン |

### `activate()`
レンダリングコンテキストを有効化する。
スクリプト内でDLLの機能を利用する前に1回実行すること。

#### 戻り値
| 型      | 説明                              |
| ------- | --------------------------------- |
| boolean | `true` なら成功、`false` なら失敗 |

### `deactivate()`
レンダリングコンテキストを無香かする。
スクリプト内でDLLの機能を利用し終わった後に1回実行すること。

#### 戻り値
なし

### `setPlaneVertex(n)`
板ポリゴンを表す頂点配列オブジェクトを利用可能にする。

板ポリゴンは横に n 分割、縦に n 分割される。
$(n + 1) ^ 2$ 個の 頂点が作成される。

#### 引数
| 名前 | 型  | 説明                              |
| ---- | --- | --------------------------------- |
| n    | int | 板ポリゴンの分割数。1以上の整数。 |

#### 戻り値
なし

#### 頂点属性
| location | 型   | 説明                    |
| -------: | ---- | ----------------------- |
|        0 | vec3 | 頂点座標 $[-1, 1]$      |
|        1 | vec2 | テクスチャ座標 $[0, 1]$ |

### `setPointVertex(n)`
n 個の点群を表す頂点配列オブジェクトを利用可能にする。

#### 引数
| 名前 | 型  | 説明                  |
| ---- | --- | --------------------- |
| n    | int | 頂点数。1以上の整数。 |

#### 戻り値
なし

#### 頂点属性
| location | 型    | 説明                            |
| -------: | ----- | ------------------------------- |
|        0 | float | 頂点番号 $(0, 1, \cdots, n-1 )$ |

### `setShader(shaderPath [, forceReload])`
シェーダープログラムを利用可能にする。

指定したシェーダーのソースファイルをコンパイル・リンクして利用可能な状態にする。
引数で指定するファイルは1個だけだが、ファイル名が同じで拡張子が対応しているファイルが見つかれば自動で読み込まれる。

シェーダープログラムのコンパイル・リンク時にエラーがあった場合はコンソールウィンドウにエラーメッセージが表示される。

コンパイル・リンクされたシェーダープログラムは一定個数キャッシュされる。

#### 引数
| 名前        | 型      | 説明                                                                    |
| ----------- | ------- | ----------------------------------------------------------------------- |
| shaderPath  | string  | シェーダーのソースファイルの絶対パス                                    |
| forceReload | boolean | `true` ならシェーダープログラムがキャッシュされていても再コンパイルする |

#### 戻り値
なし

#### シェーダーソースファイルの拡張子
| 拡張子  | 種類                           |
| ------- | ------------------------------ |
| `.vert` | 頂点シェーダー                 |
| `.frag` | フラグメントシェーダー         |
| `.tesc` | テッセレーション制御シェーダー |
| `.tese` | テッセレーション評価シェーダー |
| `.geom` | ジオメトリシェーダー           |

### `setFloat(name, x [, y [, z [, w]]])`
ユニフォーム変数を設定する。

引数に渡した個数に応じて `float`, `vec2`, `vec3`, `vec4` 型の値を設定する。

`setShader` の後に実行すること。

#### 引数
| 名前 | 型     | 説明                   |
| ---- | ------ | ---------------------- |
| name | string | ユニフォーム変数の名前 |
| x    | number | 値1                    |
| y    | number | 値2                    |
| x    | number | 値3                    |
| w    | number | 値4                    |

#### 戻り値
なし

### `setInt(name, x [, y [, z [, w]]])`
ユニフォーム変数を設定する。

引数に渡した個数に応じて `int`, `ivec2`, `ivec3`, `ivec4` 型の値を設定する。

`setShader` の後に実行すること。

#### 引数
| 名前 | 型     | 説明                   |
| ---- | ------ | ---------------------- |
| name | string | ユニフォーム変数の名前 |
| x    | int    | 値1                    |
| y    | int    | 値2                    |
| x    | int    | 値3                    |
| w    | int    | 値4                    |

#### 戻り値
なし

### `setTexture2D(unit, data, width, height)`
テクスチャをバインドする。

#### 引数
| 名前   | 型       | 説明                             |
| ------ | -------- | -------------------------------- |
| unit   | int      | テクスチャのユニット番号         |
| data   | userdata | テクスチャのBGRA配列へのポインタ |
| width  | int      | テクスチャの幅                   |
| height | int      | テクスチャの高さ                 |

#### 戻り値
なし

### `draw(mode, data, width, height [, instanceCount])`
描画する。

#### 引数
| 名前          | 型       | 説明                            |
| ------------- | -------- | ------------------------------- |
| mode          | string   | 描画するプリミティブ            |
| data          | userdata | 描画先ののBGRA配列へのポインタ  |
| width         | int      | 描画先の幅                      |
| height        | int      | 描画先の高さ                    |
| instanceCount | int      | 描画インスタンス数。1以上の整数 |

#### 戻り値
なし

#### mode
| mode                       |
| -------------------------- |
| `POINTS`                   |
| `LINE_STRIP`               |
| `LINE_LOOP`                |
| `LINES`                    |
| `LINE_STRIP_ADJACENCY`     |
| `LINES_ADJACENCY`          |
| `TRIANGLE_STRIP`           |
| `TRIANGLE_FAN`             |
| `TRIANGLES`                |
| `TRIANGLE_STRIP_ADJACENCY` |
| `TRIANGLES_ADJACENCY`      |
| `PATCHES`                  |

## ソースコードのビルド

`vendor/lua` の中に `lua5_1_4_Win32_dll8_lib` の中身を全てコピーしてください。
MSVC (VS2022)、CMake がインストールされた環境で以下のコマンドを実行してください。

```cmd
cmake --preset default
cmake --build .\build --preset release
```

## ライセンス

[MIT License](LICENSE) に基づきます。

### サードパーティーライブラリ

#### glad2

[Glad 2](https://gen.glad.sh) にて出力したコード

https://github.com/Dav1dde/glad

(WTFPL OR CC0-1.0) AND Apache-2.0

#### khrplatform.h

```
Copyright (c) 2008-2018 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
```

#### LuaBinaries 5.1.4

http://luabinaries.sourceforge.net/

```
Copyright © 2005-2021 Tecgraf/PUC-Rio and the Kepler Project.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## 更新履歴

### v0.1.0 (2024-06-30)
- 初版
