#include "GameScene.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Behavior/NavGridComponent.h"
#include "../Engine/Render/Image.h"

#include "Light.h"
#include "Map.h"
#include "Oni.h"
#include "Player.h"
#include "NPC.h"
#include "ManagerObject.h"
#include "Navigation.h"
#include "NpcSpawner.h"

// 遊ぶ側の切り替え: trueで鬼、falseでプレイヤー
static constexpr bool PLAY_AS_ONI = true;

// ゲームシーンの暗さ(仮の値。大きくすると明るくなる)
static constexpr DirectX::XMFLOAT3 NIGHT_LIGHT_COLOR{ 0.15f, 0.15f, 0.2f };    // 平行光源の色・強さ(元は1.0)
static constexpr DirectX::XMFLOAT3 NIGHT_AMBIENT_COLOR{ 0.08f, 0.08f, 0.1f };  // 環境光(元は0.25)

void GameScene::Init()
{
    RigidbodyComponent::SetDebugDrawEnabled(true);

    // シーン全体を暗くする(元の値は覚えておく)
    Light& sunLight = Image::GetLight();
    originalLight = sunLight;
    sunLight.color = NIGHT_LIGHT_COLOR;
    sunLight.ambient = NIGHT_AMBIENT_COLOR;

    CreateGround(*this);
    CreateLight(*this);
    CreatePlayer(*this, !PLAY_AS_ONI);       // 鬼で遊ぶ時は、体だけ置く
    if (PLAY_AS_ONI) CreateOni(*this);
    auto* navigation = CreateNavigation(*this);
    CreateNpcSpawner(*this, navigation->GetComponent<NavGridComponent>());
    auto* manager = CreateManagerObject(*this);
    StartMatch(manager);
}

void GameScene::Uninit()
{
    Scene::Uninit();

    // 平行光源を元に戻す
    Image::GetLight() = originalLight;
}
