#include"LightController.h"
#include"../Engine/Core/Component.h"
#include"../Engine/Core/Scene.h"
#include"../Engine/Core/GameObject.h"
#include"../Engine/Render3D/LightComponent.h"
#include"../Engine/Core/Input.h"
#include "../Engine/Core/GameEngine.h"
#include"Light.h"
#include<algorithm>


GameObject* CreateLight(Scene& scene)
{
	auto* lightObject = scene.CreateObject("Light", "Light");

	// 位置と、開始時の向き(斜め下前向き)
	lightObject->transform.position = { 0.0f, 5.0f, -3.0f };
	lightObject->transform.rotate.x = 45.0f;

	// 黄色っぽいスポットライト(明るさと届く距離は仮の値)
	auto* light = lightObject->AddComponent<LightComponent>(LightType::Spot);
	light->SetColor(1.0f, 0.8f, 0.2f);
	light->SetIntensity(3.0f);
	light->SetRange(30.0f);

	// 向きの操作(LightComponentを使うので、後に付ける)
	lightObject->AddComponent<LightController>();

	return lightObject;
}
