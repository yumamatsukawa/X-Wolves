//#include "Player.h"
//#include "../Engine/Core/Scene.h"
//#include "../Engine/Core/GameObject.h"
//#include "../Engine/Render3D/MeshRenderer.h"
//#include "../Engine/Physics/RigidbodyComponent.h"
//
//#include "PlayerController.h"
//
//
//GameObject* CreatePlayer(Scene& scene)
//{
//	GameObject* player = scene.CreateObject("Player", "Player");
//	player->transform.position	= { 0.0f, 1.0f, 0.0f };
//	player->transform.scale = { 10.0f, 10.0f, 10.0f };
//	
//	constexpr float radius = 0.5f;
//	constexpr float halfHeight = 0.5f;
//	auto* rigidbody = player->AddComponent<CapsuleRigidbodyComponent>(
//		BodyType::Dynamic,
//		radius,
//		halfHeight,
//		100.f
//	);
//
//	// 重力をつける
//	rigidbody->SetUseGravity(true);
//
//	// 回転を停止して転倒を防止する
//	rigidbody->SetFreezeRotation(true);
//
//	player->AddComponent<PlayerController>();
//
//	return player;
//}
#include "Player.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Render/Image.h"
#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;

namespace
{
	float g_PlayerYaw = 0.0f;

	constexpr float kPlayerMoveSpeed = 7.0f;
	constexpr float kTurnSpeed = 10.0f;
	constexpr float kTpsDistance = 8.0f;
	constexpr float kTpsHeight = 3.0f;
	constexpr float kTpsPitch = 0.25f;
	constexpr float kCameraFollowSmoothness = 12.0f;

	float Clamp01(float v)
	{
		if (v < 0.0f) return 0.0f;
		if (v > 1.0f) return 1.0f;
		return v;
	}

	float NormalizeAngle(float angle)
	{
		constexpr float twoPi = XM_PI * 2.0f;
		while (angle > XM_PI)  angle -= twoPi;
		while (angle < -XM_PI) angle += twoPi;
		return angle;
	}
}

GameObject* CreatePlayer(Scene& scene)
{
	Image::SetCamera3DAutoSync(false);
	g_PlayerYaw = 0.0f;

	GameObject* player = scene.CreateObject("Player", "Player");
	player->transform.position = { 0.0f, 1.0f, 0.0f };
	player->transform.scale = { 10.0f, 10.0f, 10.0f };

	constexpr float radius = 0.5f;
	constexpr float halfHeight = 0.5f;
	auto* rigidbody = player->AddComponent<CapsuleRigidbodyComponent>(
		BodyType::Dynamic,
		radius,
		halfHeight,
		100.0f
	);

	// 重力をつける
	rigidbody->SetUseGravity(true);

	// 回転を停止して転倒を防止する
	rigidbody->SetFreezeRotation(true);

	return player;
}

void UpdatePlayer(GameObject* player, GameObject* cameraObj, float dt)
{
	if (!player)
	{
		return;
	}

	// TODO: Inputクラスのキー入力関数に合わせて変更してください
	float moveForward = 0.0f; // W / S
	float moveRight = 0.0f;   // D / A

	// if (Input::IsKeyPressed('W')) moveForward += 1.0f;
	// if (Input::IsKeyPressed('S')) moveForward -= 1.0f;
	// if (Input::IsKeyPressed('D')) moveRight += 1.0f;
	// if (Input::IsKeyPressed('A')) moveRight -= 1.0f;

	XMFLOAT3 moveDir = { moveRight, 0.0f, moveForward };
	const float lenSq = moveDir.x * moveDir.x + moveDir.z * moveDir.z;
	if (lenSq > 0.0001f)
	{
		const float invLen = 1.0f / std::sqrt(lenSq);
		moveDir.x *= invLen;
		moveDir.z *= invLen;

		// 移動方向に合わせてプレイヤーの向きを更新
		const float targetYaw = std::atan2(moveDir.x, moveDir.z);
		const float diff = NormalizeAngle(targetYaw - g_PlayerYaw);
		const float turnAlpha = Clamp01(1.0f - std::exp(-kTurnSpeed * dt));
		g_PlayerYaw = NormalizeAngle(g_PlayerYaw + diff * turnAlpha);
	}

	const XMFLOAT3 velocity = {
		moveDir.x * kPlayerMoveSpeed,
		0.0f,
		moveDir.z * kPlayerMoveSpeed
	};

	auto* rigidbody = player->GetComponent<CapsuleRigidbodyComponent>();
	if (rigidbody)
	{
		// TODO: CapsuleRigidbodyComponent の速度設定関数に合わせて変更してください
		// rigidbody->SetVelocity(velocity);
		player->transform.position.x += velocity.x * dt;
		player->transform.position.z += velocity.z * dt;
	}
	else
	{
		player->transform.position.x += velocity.x * dt;
		player->transform.position.z += velocity.z * dt;
	}

	// プレイヤーの向きが変わったらTPSカメラも連動して追従
	const XMFLOAT3 playerPos = player->transform.position;
	const XMFLOAT3 playerForward = { std::sin(g_PlayerYaw), 0.0f, std::cos(g_PlayerYaw) };

	const XMFLOAT3 lookAt = {
		playerPos.x + playerForward.x * 1.0f,
		playerPos.y + kTpsHeight,
		playerPos.z + playerForward.z * 1.0f
	};

	const float horizDist = kTpsDistance * std::cos(kTpsPitch);
	const float vertDist = kTpsDistance * std::sin(kTpsPitch);

	const XMFLOAT3 desiredCamPos = {
		lookAt.x - playerForward.x * horizDist,
		lookAt.y + vertDist,
		lookAt.z - playerForward.z * horizDist
	};

	const XMFLOAT3 currentCamPos = cameraObj ? cameraObj->transform.position : desiredCamPos;
	const float camAlpha = Clamp01(1.0f - std::exp(-kCameraFollowSmoothness * dt));
	const XMFLOAT3 nextCamPos = {
		currentCamPos.x + (desiredCamPos.x - currentCamPos.x) * camAlpha,
		currentCamPos.y + (desiredCamPos.y - currentCamPos.y) * camAlpha,
		currentCamPos.z + (desiredCamPos.z - currentCamPos.z) * camAlpha
	};

	if (cameraObj)
	{
		cameraObj->transform.position = nextCamPos;
	}

	Camera& camera3D = Image::GetCamera3D();
	camera3D.position = nextCamPos;
	camera3D.target = lookAt;
	camera3D.up = { 0.0f, 1.0f, 0.0f };
}