#include "Navigation.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Behavior/NavGridComponent.h"

GameObject* CreateNavigation(Scene& scene) {
	
	GameObject* navigation = scene.CreateObject("Navigation", "Navigation");

	NavGridSettings settings;

	// 100 * 100 のGridを作成
	settings.origin = { -50.0f, 0.0f, -50.0f };		// 作成位置
	settings.size = { 100.0f, 100.0f };				// 全体の大きさ	
	settings.cellSize = 5.0f;						// 1マスの大きさ
	settings.plane = MovePlane::XZ;					// 平面の向き
	settings.agentRadius = 5.0f;					// NPCの大きさ (10.0f) の半分
	settings.baseHeight = 0.5f;						// 地面の高さ (mapのyはpos0.0f, scl 1.0f)
	settings.agentHeight = 10.0f;					// NPCの高さ (10.0f)

	// 既定値30.0fではagentHeight(10.0f)を超えるため1.0fに設定
	settings.maxStepHeight = 1.0f;					// NPCが乗り越えられる段差の高さ

	navigation->AddComponent<NavGridComponent>(settings);

	return navigation;
}

