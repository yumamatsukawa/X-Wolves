#include "PlayerController.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Core/Time.h"

void PlayerController::Init()
{ 
	speed = 200.0f;
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

	if (rigidbody) rigidbody->SetVelocity(velocity);
}
