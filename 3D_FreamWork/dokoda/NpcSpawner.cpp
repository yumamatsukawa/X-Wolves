#include "NpcSpawner.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "NpcController.h"

NpcController* CreateNpcSpawner(Scene& scene, NavGridComponent* navigation)
{
    GameObject* obj = scene.CreateObject("NpcSpawner");

    // NPCを管理するコンポーネントを追加
    NpcController* npcManager = obj->AddComponent<NpcController>();

    // ナビゲーション情報を渡す
    npcManager->Setup(navigation);

    return npcManager;
}