#include"MissionPoint.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "MissionComponent.h"

GameObject* CreateMissionPoint(Scene& scene, Transform transform, int missionId)
{
	GameObject* mp = scene.CreateObject("MissionPoint", "MissionPoint");
	mp->transform = transform;

	mp->AddComponent<MissionComponent>(missionId);

	return mp;
}
