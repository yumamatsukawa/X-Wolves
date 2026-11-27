#include "PlayerController.h"
#include "PlayerCameraController.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Core/Time.h"
#include <cmath>

void PlayerController::Init()
{ 
	speed = 20.0f;
}

void PlayerController::Update(float dt)
{
	// ポーズ中・シーン切り替え中は操作を受け付けない(dtが0なのでタイマーも進まない)
	if (Time::IsPaused()) return;

	// PlayerのRigitbodyを取得
	RigidbodyComponent* rigidbody = GetOwner()->GetComponent<RigidbodyComponent>(); if (!rigidbody) return;

	// 移動処理
	DirectX::XMFLOAT3 velocity = { 0.f, 0.f, 0.f };

	if (Input::GetKeyPress(KEY_W)) velocity.z += speed;
	if (Input::GetKeyPress(KEY_S)) velocity.z -= speed;
	if (Input::GetKeyPress(KEY_A)) velocity.x -= speed;
	if (Input::GetKeyPress(KEY_D)) velocity.x += speed;

	// カメラがある時は、前後左右をカメラの水平の向きに合わせる(無ければワールド軸のまま)
	if (auto* camera = GetOwner()->GetComponent<PlayerCameraController>())
	{
		const float yaw = camera->GetYaw();
		const float side = velocity.x;
		const float front = velocity.z;
		velocity.x = std::cos(yaw) * side + std::sin(yaw) * front;
		velocity.z = -std::sin(yaw) * side + std::cos(yaw) * front;
	}

	if (rigidbody) rigidbody->SetVelocity(velocity);
}
