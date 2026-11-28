# X-Wolves

DirectX11 + PhysX 4.1 で作った、Unity風(GameObject + Component)のC++ゲームエンジンです。2D(スプライト)と3D(メッシュ・FBXモデル)の両方に対応しています。

このリポジトリは、学生チームで開発している `3D_FreamWork` のうち、**Engine(フレームワーク本体)**を取り出して、その上でチームのゲーム **「どこだ！」(`dokoda` プロジェクト)** を作っているものです。元のリポジトリのサンプルゲーム(Sample2D / Sample3D)やデモは含まれていません。

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

`Mesh::CreateCapsule` のように実際の大きさで作った形は、`meshRenderer->SetIgnoreScale(true)` にすると `transform.scale`(親の分も含む)で引き伸ばさずに描けます(位置と回転は反映します)。`transform.scale` を当たり判定の大きさに使いながら、見た目を二重に拡大しないための設定です。

---

## ゲーム(dokoda)

作っている途中のゲームです。起動するとタイトル画面が出て、`Start` ボタンを押すとゲーム画面に切り替わります。

ゲーム画面には、100×100の地面と、5色・計10体のNPCがいます。NPCは待機と移動をくり返し、移動する時は経路探索(NavGrid / PathFollowComponent)を使います。画面は暗くしてあり、黄色のスポットライトが1つ置いてあります。当たり判定のデバッグ表示は有効になっています。

### 鬼とプレイヤーの切り替え

どちらで遊ぶかは `dokoda\GameScene.cpp` の `PLAY_AS_ONI` で決めます(書き換えたらビルドし直します)。

| 値 | 遊ぶ側 |
|---|---|
| `true` | 鬼(空中からの一人称視点。プレイヤーは体だけ置かれ、操作できません) |
| `false` | プレイヤー(後ろから見る三人称視点。地面の外には出られません) |

ゲームシーンの明るさは、同じファイルの `NIGHT_LIGHT_COLOR` と `NIGHT_AMBIENT_COLOR` で調整します。

### 操作

| 操作 | 鬼 | プレイヤー |
|---|---|---|
| マウス移動 | 視点を回す | カメラを回す |
| `W` `A` `S` `D` | 向いている方向を基準に水平移動 | カメラの向きを基準に移動 |
| マウスホイール | 奥に回すと視線の方向へ進み、手前に回すと下がる(高さは8〜30) | なし |
| `Shift` + 矢印キー | スポットライトの向きを変える | 同じ |
| 左クリック | カーソルの固定が解除された時に、固定し直す | 同じ |

ゲーム中はカーソルが隠れて画面の中央に固定されます。別のウィンドウを選ぶと解除されます。

### まだできていないこと

- 勝敗や残り時間の画面表示(`GameManager` に制限時間5分と勝敗の判定はあります)
- 鬼がプレイヤーを捕まえる処理(画面の中央に映っている物を調べる `OniController::GetLookTarget` はあります)
- ミッション地点・ユニークNPC・ギミックをゲーム画面に置くこと
- NPCの移動先が建物と重なっていないかの判定

---

## フォルダ構成

```
X-Wolves\
├── 3D_FreamWork\
│   ├── 3D_FreamWork.sln     Engineとdokodaを含むソリューション
│   ├── PhysX.props          PhysXのinclude / lib設定をまとめたプロパティシート
│   ├── Shader.hlsl          2D用シェーダー
│   ├── Shader3D.hlsl        3D用シェーダー
│   ├── Shader3DSkin.hlsl    骨で曲げるFBX用シェーダー
│   ├── Shader3DCommon.hlsli 3Dシェーダー共通の計算
│   ├── Shader3DWater.hlsl   水面用シェーダー
│   ├── Assets\              画像・音声・モデルなどを置く場所(ボタン画像と Data\Missions.csv)
│   ├── Engine\              エンジン本体(静的ライブラリ。Engine.vcxproj)
│   └── dokoda\              ゲーム「どこだ！」(dokoda.vcxproj)
├── ThirdParty\
│   ├── PhysX\               PhysX 4.1(ビルド済みのinclude / lib / dll)
│   └── ufbx\                FBX読み込みライブラリ
└── .gitignore
```

- `Assets\` は、ビルド時にEngineが出力フォルダへコピーする素材置き場です。ゲームで使う素材はここに入れます。
- `ThirdParty\` は `3D_FreamWork\` の1つ上にあります。`PhysX.props` と `Engine.vcxproj` がこの位置を前提にしているので、フォルダの位置を動かさないでください。

---

## ビルド方法

必要なもの: Windows、Visual Studio 2022(「C++によるデスクトップ開発」ワークロード)。

1. `3D_FreamWork\3D_FreamWork.sln` をVisual Studioで開く
2. 構成を `Debug` または `Release`、プラットフォームを **x64** にする(x64のみ対応)
3. 「ビルド」→「ソリューションのビルド」

できあがるもの: `3D_FreamWork\x64\Debug\` の `Engine.lib` と `dokoda.exe`(Releaseなら `x64\Release\`)。ビルド時にPhysXのDLL・シェーダー・`Assets\` も同じ出力フォルダへコピーされます。これらのビルド成果物はGitには含めません(`.gitignore`で除外)。

別のゲームからEngineを使う時は、ゲーム用のプロジェクトを同じソリューションに追加し、`Engine.vcxproj` への `ProjectReference` と `PhysX.props` のImportを設定してください。

---

## 元のリポジトリについて

このエンジンは、学生チームの `3D_FreamWork` から切り出したものです。

## ライセンスについて

- PhysX 4.1(NVIDIA)は `ThirdParty\PhysX\` に入っています。BSD 3-Clause型のライセンスで、条文は `ThirdParty\PhysX\LICENSE.txt` にあります。
- ufbx は `ThirdParty\ufbx\` に入っています。条文は同じフォルダの `LICENSE` にあります。
- このリポジトリ自体のライセンスは、まだ決めていません。
