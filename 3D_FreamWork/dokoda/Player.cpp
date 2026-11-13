#include "Player.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"

#include "PlayerController.h"


GameObject* CreatePlayer(Scene& scene)
{
	GameObject* player = scene.CreateObject("Player", "Player");
	player->transform.position	= { 0.0f, 0.0f, 0.0f };
	player->transform.scale = { 100.0f, 100.0f, 100.0f };

	player->AddComponent<MeshRenderer>(Mesh::CreateCube());

	// “–‚½‚è”»’è+•¨—‰‰ŽZ(PhysX)
	auto* rigidbody = player->AddComponent<BoxRigidbodyComponent>(BodyType::Dynamic, player->transform.scale, 100.f);
	
	// d—Í‚ð‚Â‚¯‚é
	rigidbody->SetUseGravity(true);

	// ‰ñ“]‚ð’âŽ~‚µ‚Ä“]“|‚ð–hŽ~‚·‚é
	rigidbody->SetFreezeRotation(true);

	player->AddComponent<PlayerController>();
}