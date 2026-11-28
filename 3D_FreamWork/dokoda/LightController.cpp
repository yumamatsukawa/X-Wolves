#include"LightController.h"
#include"../Engine/Core/Component.h"
#include"../Engine/Render3D/LightComponent.h"
#include"../Engine/Core/Input.h"
#include "../Engine/Core/GameEngine.h"
#include<algorithm>
#include<cmath>

void LightController::Init() 
{
	light = GetOwner()->GetComponent<LightComponent>();
	ApplyDirection();
}

void LightController::ApplyDirection()
{
	if (!light) return;

	// rotate(度)から向きを作る: y=0で+Z、yが増えると右、xが増えると下
	const Transform& transform = GetOwner()->transform;
	const float pitch = DirectX::XMConvertToRadians(transform.rotate.x);
	const float yaw = DirectX::XMConvertToRadians(transform.rotate.y);
	const float cosPitch = std::cos(pitch);

	light->SetDirection({ std::sin(yaw) * cosPitch, -std::sin(pitch), std::cos(yaw) * cosPitch });
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

	//ライトの向きに反映する
	ApplyDirection();
}