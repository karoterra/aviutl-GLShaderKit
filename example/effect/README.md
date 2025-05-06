# GLShaderKit_effect.anm

シェーダーを使用してエフェクトをかけるアニメーション効果です。

| 種類             | 項目名 | 説明                                        |
| ---------------- | ------ | ------------------------------------------- |
| トラックバー     | track0 | スクリプトで使用                            |
| トラックバー     | track1 | スクリプトで使用                            |
| トラックバー     | track2 | スクリプトで使用                            |
| トラックバー     | track3 | スクリプトで使用                            |
| ファイル         | 参照   | 使用するシェーダー                          |
| チェックボックス | 再読込 | ON にすると毎回シェーダーを再コンパイルする |

## GLShaderKit_effect_01

- [頂点シェーダー](GLShaderKit_effect_01.vert)
- [フラグメントシェーダー](GLShaderKit_effect_01.frag)
- [使用画像](https://pixabay.com/photos/buttons-colored-buttons-1821331/)

**適用前**
![GLShaderKit_effect で シェーダーとして GLShaderKit_effect_01 を使用する前の出力例](/assets/GLShaderKit_effect_01_before.webp)

**適用後**
![GLShaderKit_effect で シェーダーとして GLShaderKit_effect_01 を使用した後の出力例](/assets/GLShaderKit_effect_01_after.webp)

## GLShaderKit_effect_02

- [フラグメントシェーダー](GLShaderKit_effect_02.frag)

トラックバーの track0, 1, 2 に応じて画像のRGBを調整する。

GLShaderKit のデフォルト頂点シェーダーを使用するため、頂点シェーダーファイルはなし。

## GLShaderKit_effect_03

- [フラグメントシェーダー](GLShaderKit_effect_03.frag)

トラックバーの track0, 1, 2 に応じて画像にグリッチをかける。

GLShaderKit のデフォルト頂点シェーダーを使用するため、頂点シェーダーファイルはなし。
