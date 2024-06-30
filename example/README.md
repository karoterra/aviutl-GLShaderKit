# GLShaderKit サンプル

GLShaderKit を利用したスクリプト、シェーダーのサンプルです。

## [GLShaderKit_info.obj](GLShaderKit_info.obj)

バージョン情報などの取得できる各種情報を表示するカスタムオブジェクトです。

![GLShaderKit_info.obj の出力例](/assets/GLShaderKit_info.webp)

## [GLShaderKit_draw.obj](GLShaderKit_draw.obj)

シェーダーを使用して描画するカスタムオブジェクトです。

| 種類             | 項目名 | 説明                                        |
| ---------------- | ------ | ------------------------------------------- |
| トラックバー     | 幅     | カスタムオブジェクトの幅 (ピクセル)         |
| トラックバー     | 高さ   | カスタムオブジェクトの高さ (ピクセル)       |
| トラックバー     | track2 | 未使用                                      |
| トラックバー     | track3 | 未使用                                      |
| ファイル         | 参照   | 使用するシェーダー                          |
| チェックボックス | 再読込 | ON にすると毎回シェーダーを再コンパイルする |

### GLShaderKit_draw_01

- [頂点シェーダー](GLShaderKit_draw_01.vert)
- [フラグメントシェーダー](GLShaderKit_draw_01.frag)

![GLShaderKit_draw で シェーダーとして GLShaderKit_draw_01 を使用したときの出力例](/assets/GLShaderKit_draw_01.webp)

### GLShaderKit_draw_02

- [頂点シェーダー](GLShaderKit_draw_02.vert)
- [フラグメントシェーダー](GLShaderKit_draw_02.frag)

![GLShaderKit_draw で シェーダーとして GLShaderKit_draw_02 を使用したときの出力例](/assets/GLShaderKit_draw_02.webp)

### GLShaderKit_draw_03

- [頂点シェーダー](GLShaderKit_draw_03.vert)
- [フラグメントシェーダー](GLShaderKit_draw_03.frag)

![GLShaderKit_draw で シェーダーとして GLShaderKit_draw_03 を使用したときの出力例](/assets/GLShaderKit_draw_03.webp)

### GLShaderKit_draw_04

- [頂点シェーダー](GLShaderKit_draw_04.vert)
- [フラグメントシェーダー](GLShaderKit_draw_04.frag)

![GLShaderKit_draw で シェーダーとして GLShaderKit_draw_04 を使用したときの出力例](/assets/GLShaderKit_draw_04.webp)

## [GLShaderKit_drawPoints.obj](GLShaderKit_drawPoints.obj)

シェーダーを使用して点群を描画するカスタムオブジェクトです。

| 種類             | 項目名     | 説明                                        |
| ---------------- | ---------- | ------------------------------------------- |
| トラックバー     | 頂点数     | 頂点の数                                    |
| トラックバー     | 頂点サイズ | 頂点の大きさ                                |
| トラックバー     | track2     | 未使用                                      |
| トラックバー     | track3     | 未使用                                      |
| ファイル         | 参照       | 使用するシェーダー                          |
| チェックボックス | 再読込     | ON にすると毎回シェーダーを再コンパイルする |

### GLShaderKit_drawPoints_01

- [頂点シェーダー](GLShaderKit_drawPoints_01.vert)
- [フラグメントシェーダー](GLShaderKit_drawPoints_01.frag)

![GLShaderKit_drawPoints で シェーダーとして GLShaderKit_drawPoints_01 を使用したときの出力例](/assets/GLShaderKit_drawPoints_01.webp)

### GLShaderKit_drawPoints_02

- [頂点シェーダー](GLShaderKit_drawPoints_02.vert)
- [フラグメントシェーダー](GLShaderKit_drawPoints_02.frag)

![GLShaderKit_drawPoints で シェーダーとして GLShaderKit_drawPoints_02 を使用したときの出力例](/assets/GLShaderKit_drawPoints_02.webp)

### GLShaderKit_drawPoints_03

- [頂点シェーダー](GLShaderKit_drawPoints_03.vert)
- [フラグメントシェーダー](GLShaderKit_drawPoints_03.frag)

![GLShaderKit_drawPoints で シェーダーとして GLShaderKit_drawPoints_03 を使用したときの出力例](/assets/GLShaderKit_drawPoints_03.webp)

## [GLShaderKit_effect.anm](GLShaderKit_effect.anm)

シェーダーを使用してエフェクトをかけるアニメーション効果です。

| 種類             | 項目名 | 説明                                        |
| ---------------- | ------ | ------------------------------------------- |
| トラックバー     | track0 | 未使用                                      |
| トラックバー     | track1 | 未使用                                      |
| トラックバー     | track2 | 未使用                                      |
| トラックバー     | track3 | 未使用                                      |
| ファイル         | 参照   | 使用するシェーダー                          |
| チェックボックス | 再読込 | ON にすると毎回シェーダーを再コンパイルする |

### GLShaderKit_effect_01

- [頂点シェーダー](GLShaderKit_effect_01.vert)
- [フラグメントシェーダー](GLShaderKit_effect_01.frag)
- [使用画像](https://pixabay.com/photos/buttons-colored-buttons-1821331/)

| 適用前                                                                                                                          | 適用後                                                                                                                         |
| ------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ |
| ![GLShaderKit_effect で シェーダーとして GLShaderKit_effect_01 を使用する前の出力例](/assets/GLShaderKit_effect_01_before.webp) | ![GLShaderKit_effect で シェーダーとして GLShaderKit_effect_01 を使用した後の出力例](/assets/GLShaderKit_effect_01_after.webp) |

## [GLShaderKit_effectInstanced.anm](GLShaderKit_effectInstanced.anm)

シェーダーを使用してエフェクトをかけるアニメーション効果です。
一度に複数のインスタンスを描画します。

| 種類             | 項目名 | 説明                                        |
| ---------------- | ------ | ------------------------------------------- |
| トラックバー     | 個数   | インスタンス数                              |
| トラックバー     | track1 | 未使用                                      |
| トラックバー     | track2 | 未使用                                      |
| トラックバー     | track3 | 未使用                                      |
| ファイル         | 参照   | 使用するシェーダー                          |
| チェックボックス | 再読込 | ON にすると毎回シェーダーを再コンパイルする |

### GLShaderKit_effectInstanced_01

- [頂点シェーダー](GLShaderKit_effectInstanced_01.vert)
- [フラグメントシェーダー](GLShaderKit_effectInstanced_01.frag)
- [使用画像](https://pixabay.com/photos/buttons-colored-buttons-1821331/)

| 適用前                                                                                                                                            | 適用後                                                                                                                                           |
| ------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| ![GLShaderKit_effectInstanced で シェーダーとして GLShaderKit_effectInstanced_01 を使用する前の出力例](/assets/GLShaderKit_effectInstanced_01_before.webp) | ![GLShaderKit_effectInstanced で シェーダーとして GLShaderKit_effectInstanced_01 を使用した後の出力例](/assets/GLShaderKit_effectInstanced_01_after.webp) |
