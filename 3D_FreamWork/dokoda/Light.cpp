#include"LightController.h"
#include"../Engine/Core/Component.h"
#include"../Engine/Render3D/LightComponent.h"
#include"../Engine/Core/Input.h"
#include "../Engine/Core/GameEngine.h"
#include"Light.h"
#include<algorithm>


GameObject* CreateLight(Scene& scene)
{
	auto* lightObject = scene.CreateObject();

	lightObject->AddComponent<LightComponent>();
	lightObject->AddComponent<LightController>();

	return lightObject;
}