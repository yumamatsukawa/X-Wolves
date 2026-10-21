# X-Wolves

DirectX11 + PhysX 4.1 で作った、Unity風(GameObject + Component)のC++ゲームエンジンです。2D(スプライト)と3D(メッシュ・FBXモデル)の両方に対応しています。

このリポジトリは、学生チームで開発している `3D_FreamWork` のうち、**Engine(フレームワーク本体)だけ**を取り出したものです。サンプルゲーム(Sample2D / Sample3D)やデモは含まれていません。

---

## Engineでできること

`GameObject` は中身のない入れ物で、`Component`(部品)を `AddComponent<T>()` で付けて見た目や動きを作ります(継承ではなく組み合わせ)。Engineは機能ごとにフォルダが分かれています。

| フォルダ | 主な内容 |
|---|---|
| `Core` | ゲームの起動(GameEngine)、Scene / SceneManager、GameObject / Component / Transform、Input、Time / Timer、EventBus、ObjectPool、Resources、数学関数(MathUtil) |
| `Render` | 2D描画。SpriteRenderer、Camera、CameraFollowComponent、CameraShake、画面エフェクト(ScreenEffect)、パーティクル(ParticleRenderer)、Texture / Image など |
| `Render3D` | 3D描画。MeshRenderer、FBXモデル(Model / ModelRenderer / SkinMesh)、地形(Terrain)、水面(WaterRenderer)、ライト、影、マテリアル、LOD、描画カウンタ |
| `Physics` | PhysXによる物理演算。RigidbodyComponent、Collider、衝突の通知 |
| `Audio` | BGM / SEの再生、AudioSourceComponent、AudioListenerComponent |
| `UI` | 文字(Text / TextRenderer)、ゲージ、スライダー、ボタン、Anchor |
| `Animation` | イージング(Ease)、TweenComponent、スプライト / モデルのアニメーション |
| `Transition` | シーン切り替えの演出(フェード、スプライトを使った切り替え) |
| `Behavior` | 移動・追いかけ・ホーミング・視界判定(VisionComponent)・経路探索(NavGrid / PathFollowComponent) |
| `Data` | CSVの読み込み(Csv)、セーブデータ(SaveData) |
| `ThirdParty`(Engine内) | WICTextureLoader11(画像読み込み) |

使い方の細かい説明(各機能のサンプルコードなど)は、元のリポジトリ `3D_FreamWork` の README にあります。

---

## フォルダ構成

```
X-Wolves\
├── 3D_FreamWork\
│   ├── 3D_FreamWork.sln     Engineだけを含むソリューション
│   ├── PhysX.props          PhysXのinclude / lib設定をまとめたプロパティシート
│   ├── Shader.hlsl          2D用シェーダー
│   ├── Shader3D.hlsl        3D用シェーダー
│   ├── Shader3DSkin.hlsl    骨で曲げるFBX用シェーダー
│   ├── Shader3DCommon.hlsli 3Dシェーダー共通の計算
│   ├── Shader3DWater.hlsl   水面用シェーダー
│   ├── Assets\              画像・音声・モデルなどを置く場所(今は空)
│   └── Engine\              エンジン本体(静的ライブラリ。Engine.vcxproj)
├── ThirdParty\
│   ├── PhysX\               PhysX 4.1(ビルド済みのinclude / lib / dll)
│   └── ufbx\                FBX読み込みライブラリ
└── .gitignore
```

- `Assets\` は、ビルド時にEngineが出力フォルダへコピーする素材置き場です。ゲームを作る時はここに素材を入れます。
- `ThirdParty\` は `3D_FreamWork\` の1つ上にあります。`PhysX.props` と `Engine.vcxproj` がこの位置を前提にしているので、フォルダの位置を動かさないでください。

---

## ビルド方法

必要なもの: Windows、Visual Studio 2022(「C++によるデスクトップ開発」ワークロード)。

1. `3D_FreamWork\3D_FreamWork.sln` をVisual Studioで開く
2. 構成を `Debug` または `Release`、プラットフォームを **x64** にする(x64のみ対応)
3. 「ビルド」→「ソリューションのビルド」

できあがるもの: `3D_FreamWork\x64\Debug\Engine.lib`(Releaseなら `x64\Release\`)。ビルド時にPhysXのDLL・シェーダー・`Assets\` も同じ出力フォルダへコピーされます。これらのビルド成果物はGitには含めません(`.gitignore`で除外)。

自分のゲームから使う時は、ゲーム用のプロジェクトを同じソリューションに追加し、`Engine.vcxproj` への `ProjectReference` と `PhysX.props` のImportを設定してください。

---

## 元のリポジトリについて

このエンジンは、学生チームの `3D_FreamWork` から切り出したものです。

## ライセンスについて

- PhysX 4.1(NVIDIA)は `ThirdParty\PhysX\` に入っています。BSD 3-Clause型のライセンスで、条文は `ThirdParty\PhysX\LICENSE.txt` にあります。
- ufbx は `ThirdParty\ufbx\` に入っています。条文は同じフォルダの `LICENSE` にあります。
- このリポジトリ自体のライセンスは、まだ決めていません。
