#include"UniqueNpc.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include"../Engine/Render3D/MeshRenderer.h"
#include"../Engine/Core/MathUtil.h"

GameObject* CreateUniqueNpc(Scene& scene, UniqueNpcType type, XMFLOAT3 pos)
{
	GameObject* npc = scene.CreateObject("UniqueNpc", "UniqueNpc");
	npc->SetTag(std::string("UniqueNPC"));
	npc->transform.position = pos;
	npc->transform.scale = { 10.0f,10.0f,10.0f };
	switch (type)
	{
	case UniqueNpcType::ZombieDog:
		break;
	case UniqueNpcType::Witch:
		break;
	case UniqueNpcType::Vampire:
		break;
	case UniqueNpcType::Reaper:
		break;
	case UniqueNpcType::Werewolf:
		break;
	case UniqueNpcType::Necromancer:
		break;
	case UniqueNpcType::CryingChild:
		break;
	case UniqueNpcType::GhostA:
		break;
	case UniqueNpcType::GhostB:
		break;
	case UniqueNpcType::CursedDoll:
		break;
	default:
		break;
	}
	
	return npc;
}