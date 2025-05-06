# GLShaderKit_effectInstanced.anm

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

## GLShaderKit_effectInstanced_01

- [頂点シェーダー](GLShaderKit_effectInstanced_01.vert)
- [フラグメントシェーダー](GLShaderKit_effectInstanced_01.frag)
- [使用画像](https://pixabay.com/photos/buttons-colored-buttons-1821331/)

**適用前**
![GLShaderKit_effectInstanced で シェーダーとして GLShaderKit_effectInstanced_01 を使用する前の出力例](/assets/GLShaderKit_effectInstanced_01_before.webp)

**適用後**
![GLShaderKit_effectInstanced で シェーダーとして GLShaderKit_effectInstanced_01 を使用した後の出力例](/assets/GLShaderKit_effectInstanced_01_after.webp)
