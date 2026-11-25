#include "ManagerObject.h"

#include "GameManager.h"
#include "EventManager.h"
#include "MissionManager.h"

#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"

GameObject* CreateManagerObject(Scene& scene)
{
	GameObject* managerObject = scene.CreateObject("ManagerObject", "Manager");

	if (!managerObject) return nullptr;

	// ManagerComponentを追加 (順番に注意)
	managerObject->AddComponent<GameManager>();
	managerObject->AddComponent<EventManager>();
	managerObject->AddComponent<MissionManager>();

	return managerObject;
}

bool StartMatch(GameObject* managerObject)
{
	if (!managerObject) return false;

	// 各Managerの取得
	GameManager* gameManager = managerObject->GetComponent<GameManager>();
	EventManager* eventManager = managerObject->GetComponent<EventManager>();
	MissionManager* missionManager = managerObject->GetComponent<MissionManager>();

	if (!gameManager || !eventManager || !missionManager) return false;

	// 状態のリセット
	eventManager->Reset();
	missionManager->Reset();
	gameManager->Reset();

	// ゲーム開始
	gameManager->StartGame();

	return true;
}