#include"NPC.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include"../Engine/Render3D/MeshRenderer.h"

GameObject* CreateNpc(Scene& scene)
{
	GameObject* npc = scene.CreateObject("Npc", "Npc");
	npc->transform.position = {0.0f,5.0f,0.0f};
	npc->transform.scale = {10.0f,10.0f,10.0f};

	auto* meshRenderer = npc->AddComponent<MeshRenderer>(Mesh::CreateCube());
	meshRenderer->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	
	return npc;
}
