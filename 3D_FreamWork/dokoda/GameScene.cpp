#include "GameScene.h"
#include "Map.h"
#include "OniController.h"
#include "Player.h"
#include "NPC.h"

void GameScene::Init()
{
    CreateGround(*this);
    CreatePlayer(*this);
    CreateCamera(*this);
    CreateNpc(*this);
}

void GameScene::Uninit()
{
    Scene::Uninit();
}
