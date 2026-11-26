#include "GameScene.h"
#include "../Engine/Physics/RigidbodyComponent.h"

#include "Map.h"
#include "OniController.h"
#include "Player.h"
#include "NPC.h"
#include "ManagerObject.h"

void GameScene::Init()
{
    RigidbodyComponent::SetDebugDrawEnabled(true);

    CreateGround(*this);
    CreatePlayer(*this);
    CreateCamera(*this, CameraMode::Overhead);
    //CreateNpc(*this);
    auto* manager = CreateManagerObject(*this);
    StartMatch(manager);
}

void GameScene::Uninit()
{
    Scene::Uninit();
}
