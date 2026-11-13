#include"NPC.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include"../Engine/Render3D/MeshRenderer.h"

GameObject* CreateNpc(Scene& scene)
{

	GameObject* npc = scene.CreateObject("Npc", "Npc");
	npc->transform.position = {0.0f,0.0f,0.0f};
	npc->transform.scale = {100.0f,100.0f,100.0f};

	auto* meshRenderer = npc->AddComponent<MeshRenderer>(Mesh::CreateCube());
	









}
