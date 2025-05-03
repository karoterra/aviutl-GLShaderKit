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
   - `GLShaderKit.dll` ： `exedit.auf` と同じ場所
   - `GLShaderKit.ini` ： `GLShaderKit.dll` と同じ場所

> [!NOTE]
> `GLShaderKit.dll` は `script` フォルダ内に置いても動作しますが、スクリプトを作者ごとにフォルダ分けする場合などを想定して推奨は `exedit.auf` と同じ場所とします。
> 分かっている人は `script` フォルダ内や `module` フォルダ内でも問題ありません。

> [!WARNING]
> GLShaderKit v0.2.0 から更新する場合は `GLShaderKit.auf`, `GLShaderKit.ini`, `GLShaderKit.lua` を削除してから導入してください。

## 使用例

GLShaderKit を使用したスクリプト・シェーダーのサンプルは [example](example) を参照してください。

## 設定

`GLShaderKit.dll` と同じフォルダに置いた `GLShaderKit.ini` は設定ファイルです。

### 設定項目
| 項目名                | 説明                               |
| --------------------- | ---------------------------------- |
| `shaderCacheCapacity` | シェーダープログラムのキャッシュ数 |

## スクリプト開発者へ

GLShaderKitのリファレンスは[こちら](/docs/api/README.md)を参照してください。

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

### v0.4.0 (2024-09-16)
- `setUInt`, `setMatrix` の追加 ([#13](https://github.com/karoterra/aviutl-GLShaderKit/pull/13))
- デフォルト頂点シェーダーの追加 ([#14](https://github.com/karoterra/aviutl-GLShaderKit/pull/14))

### v0.3.1 (2024-09-14)
- 複数のテクスチャを使用する際に意図したテクスチャにならないことがある問題を修正 ([#11](https://github.com/karoterra/aviutl-GLShaderKit/pull/11))

### v0.3.0 (2024-08-18)
- Radeon 環境だと AviUtl を閉じれないことがある問題を再修正 ([#8](https://github.com/karoterra/aviutl-GLShaderKit/pull/8))
  - auf から dll に戻した

### v0.2.0 (2024-08-18)
- Radeon 環境だと AviUtl を閉じれないことがある問題を修正 ([#7](https://github.com/karoterra/aviutl-GLShaderKit/pull/7))
  - dll から auf に変更

### v0.1.0 (2024-06-30)
- 初版
