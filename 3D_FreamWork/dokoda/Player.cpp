#include "Player.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "PlayerController.h"
#include "PlayerCameraController.h"

GameObject* CreatePlayer(Scene& scene, bool controllable)
{
	GameObject* player = scene.CreateObject("Player", "Player");
	player->transform.position	= { 0.0f, 1.0f, 0.0f };
	player->transform.scale = { 1.0f, 2.0f, 1.0f };

	auto* rigidbody = player->AddComponent<CapsuleRigidbodyComponent>(
		BodyType::Dynamic,
		player->transform.scale.x / 2,
		player->transform.scale.y,
		100.f,
		true
	);
	// 重力をつける
	rigidbody->SetUseGravity(false);
	// 回転を停止して転倒を防止する
	rigidbody->SetFreezeRotation(true);

	auto* meshRenderer = player->AddComponent<MeshRenderer>(Mesh::CreateCapsule(player->transform.scale.x / 2, player->transform.scale.y));
	meshRenderer->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	meshRenderer->SetIgnoreScale(true);

	// 操作する時だけ、カメラと操作を付ける(カメラの向きを同じフレームの移動に使うので、カメラが先)
	if (controllable)
	{
		player->AddComponent<PlayerCameraController>();
		player->AddComponent<PlayerController>();
	}

	return player;
}