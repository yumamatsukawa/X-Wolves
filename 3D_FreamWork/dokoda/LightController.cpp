#include"LightController.h"
#include"../Engine/Core/Component.h"
#include"../Engine/Render3D/LightComponent.h"
#include"../Engine/Core/Input.h"
#include "../Engine/Core/GameEngine.h"

#include<algorithm>

GameObject* CreateLight(Scene& scene)
{
	auto* lightObject = scene.CreateObject();

	lightObject->AddComponent<LightComponent>();
	lightObject->AddComponent<LightController>();

	return lightObject;
}





void LightController::Init() 
{
	light = GetOwner()->GetComponent<LightComponent>();

}
void LightController::Update(float dt) 
{
	Transform& transform = GetOwner()->transform;

	float rotateAmount = rotateSpeed * dt;

	//左右回転
	if (Input::GetKeyPress(KEY_SHIFT) && Input::GetKeyPress(KEY_LEFT))
	{
		transform.rotate.y -= rotateAmount;
	}
	if (Input::GetKeyPress(KEY_SHIFT) && Input::GetKeyPress(KEY_RIGHT))
	{
		transform.rotate.y += rotateAmount;
	}

	//上下回転
	if (Input::GetKeyPress(KEY_SHIFT) && Input::GetKeyPress(KEY_UP))
	{
		transform.rotate.x -= rotateAmount;
	}
	if (Input::GetKeyPress(KEY_SHIFT) && Input::GetKeyPress(KEY_DOWN))
	{
		transform.rotate.x += rotateAmount;
	}

	//真上、真下まで回りこまないように制限
	if (transform.rotate.x < minPitch)
	{
		transform.rotate.x = minPitch;
	}

	if (transform.rotate.x > maxPitch)
	{
		transform.rotate.x = maxPitch;
	}

}