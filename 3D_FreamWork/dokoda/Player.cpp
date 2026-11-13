#include "Player.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"

#include "PlayerController.h"


GameObject* CreatePlayer(Scene& scene)
{
	GameObject* player = scene.CreateObject("Player", "Player");
	player->transform.position	= { 0.0f, 1.0f, 0.0f };
	player->transform.scale = { 10.0f, 10.0f, 10.0f };

	auto* mesh = player->AddComponent<MeshRenderer>(Mesh::CreateCube());
	mesh->SetColor(0.0f,0.0f,0.0f,1.0f);

	// 当たり判定+物理演算(PhysX)
	auto* rigidbody = player->AddComponent<BoxRigidbodyComponent>(BodyType::Dynamic, player->transform.scale, 100.f);
	
	// 重力をつける
	rigidbody->SetUseGravity(true);

	// 回転を停止して転倒を防止する
	rigidbody->SetFreezeRotation(true);

	player->AddComponent<PlayerController>();

	return player;
}