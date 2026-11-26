#include"NPC.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include"../Engine/Render3D/MeshRenderer.h"
#include"../Engine/Core/MathUtil.h"

GameObject* CreateNpc(Scene& scene, NpcType type, XMFLOAT3 pos)
{
	GameObject* npc = scene.CreateObject("Npc", "Npc");
	npc->SetTag(std::string("NPC"));
	npc->transform.position = pos;
	npc->transform.scale = {10.0f,10.0f,10.0f};
	switch (type)
	{
	case NpcType::Zombie:
		break;
	case NpcType::Vampire:
		break;
	case NpcType::Ghost:
		break;
	case NpcType::Witch:
		break;
	case NpcType::Reaper:
		break;
	default:
		break;
	}
	int randomColor = Math::RandomRange(0, static_cast<int>(ColorType::Count)-1);
	auto* meshRenderer = npc->AddComponent<MeshRenderer>(Mesh::CreateCube());
	switch (static_cast<ColorType>(randomColor))
	{
	case ColorType::Red:
		meshRenderer->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ColorType::Blue:
		meshRenderer->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case ColorType::Green:
		meshRenderer->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case ColorType::Yellow:
		meshRenderer->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	}
	return npc;
}