#include "Player.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"

#include "PlayerController.h"


GameObject* CreatePlayer(Scene& scene)
{
	GameObject* player = scene.CreateObject("Player", "Player");
	player->transform.position	= { 0.0f, 5.0f, 0.0f };
	player->transform.scale = { 1.0f, 1.0f, 1.0f };
	
	constexpr float radius = 0.5f;
	constexpr float height = 2.0f;
	auto* rigidbody = player->AddComponent<CapsuleRigidbodyComponent>(
		BodyType::Dynamic,
		radius,
		height,
		100.f
	);

	auto* meshRenderer = player->AddComponent<MeshRenderer>(Mesh::CreateCapsule(radius, height));
	meshRenderer->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 重力をつける
	rigidbody->SetUseGravity(true);

	// 回転を停止して転倒を防止する
	rigidbody->SetFreezeRotation(true);

	player->AddComponent<PlayerController>();

	return player;
}