#include"NPC.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include"../Engine/Render3D/MeshRenderer.h"
#include"../Engine/Core/MathUtil.h"
#include "../Engine/Physics/RigidbodyComponent.h"

GameObject* CreateNpc(Scene& scene, NpcType type, XMFLOAT3 pos)
{
	GameObject* npc = scene.CreateObject("Npc", "Npc");
	npc->SetTag(std::string("NPC"));
	npc->transform.position = pos;
	auto* meshRenderer = npc->AddComponent<MeshRenderer>(Mesh::CreateCapsule(npc->transform.scale.x / 2, npc->transform.scale.y));
	switch (type)
	{
	case NpcType::Zombie:
		meshRenderer->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case NpcType::Vampire:
		meshRenderer->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case NpcType::Ghost:
		meshRenderer->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case NpcType::Witch:
		meshRenderer->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case NpcType::Reaper:
		meshRenderer->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}

	auto* rigidbody = npc->AddComponent<CapsuleRigidbodyComponent>(
		BodyType::Dynamic,
		npc->transform.scale.x / 2,
		npc->transform.scale.y,
		100.f,
		100.f,
		true
	);
	// 重力をつける
	rigidbody->SetUseGravity(false);
	// 回転を停止して転倒を防止する
	rigidbody->SetFreezeRotation(true);
	
	
	return npc;
}