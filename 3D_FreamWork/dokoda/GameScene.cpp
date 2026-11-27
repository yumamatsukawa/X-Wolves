#include "GameScene.h"
#include "../Engine/Physics/RigidbodyComponent.h"

#include "Map.h"
#include "Oni.h"
#include "Player.h"
#include "NPC.h"
#include "ManagerObject.h"

// 遊ぶ側の切り替え: trueで鬼、falseでプレイヤー
static constexpr bool PLAY_AS_ONI = false;

void GameScene::Init()
{
    RigidbodyComponent::SetDebugDrawEnabled(true);

    CreateGround(*this);
    CreatePlayer(*this, !PLAY_AS_ONI);       // 鬼で遊ぶ時は、体だけ置く
    if (PLAY_AS_ONI) CreateOni(*this);
    //CreateNpc(*this);
    auto* manager = CreateManagerObject(*this);
    StartMatch(manager);
}

void GameScene::Uninit()
{
    Scene::Uninit();
}
