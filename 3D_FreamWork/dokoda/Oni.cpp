
#include "Oni.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Render/Image.h"
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

using namespace DirectX;

namespace
{
	float g_OniYaw = 0.0f;
	float g_OniPitch = 0.0f;
	float g_OniEyeHeight = 1.7f;
	float g_OniZoomOffset = 0.0f;

	constexpr float kMoveSpeed = 8.0f;
	constexpr float kVerticalSpeed = 6.0f;
	constexpr float kMouseSensitivity = 0.003f;
	constexpr float kWheelSensitivity = 0.25f;

	float ClampFloat(float v, float minVal, float maxVal)
	{
		if (v < minVal) return minVal;
		if (v > maxVal) return maxVal;
		return v;
	}

	constexpr float kVisionRange = 25.0f;       // 視認できる最大距離
	constexpr float kVisionFovDeg = 90.0f;      // 視野角（度：例 90度なら左右45度ずつ）

}

GameObject* CreateOni(Scene& scene)
{
	Image::SetCamera3DAutoSync(false);

	GameObject* oni = scene.CreateObject("Oni", "Oni");
	oni->transform.position = { 0.0f, 1.0f, 0.0f };

	constexpr float radius = 0.5f;
	constexpr float halfHeight = 0.5f;
	auto* rigidbody = oni->AddComponent<CapsuleRigidbodyComponent>(
		BodyType::Dynamic,
		radius,
		halfHeight,
		100.0f
	);
	rigidbody->SetUseGravity(false);
	rigidbody->SetFreezeRotation(true);

	return oni;
}

void UpdateOni(GameObject* oni, GameObject* cameraObj, float dt)
{
	if (!oni)
	{
		return;
	}

	// TODO: Inputクラスのマウス移動量・ホイール取得関数に合わせて変更してください
	float mouseDX = 0.0f;
	float mouseDY = 0.0f;
	float wheel = 0.0f;

	g_OniYaw += mouseDX * kMouseSensitivity;
	g_OniPitch -= mouseDY * kMouseSensitivity;

	

	constexpr float maxPitch = XM_PIDIV2 - 0.05f;
	g_OniPitch = ClampFloat(g_OniPitch, -maxPitch, maxPitch);

	// ホイールで前後方向のズーム調整
	g_OniZoomOffset = ClampFloat(g_OniZoomOffset + wheel * kWheelSensitivity, -1.0f, 2.5f);

	

	// TODO: Inputクラスのキー入力関数に合わせて変更してください
	float moveForward = 0.0f; // W / S
	float moveRight = 0.0f;   // D / A
	float moveUp = 0.0f;      // E / Q

	// if (Input::IsKeyPressed('W')) moveForward += 1.0f;
	// if (Input::IsKeyPressed('S')) moveForward -= 1.0f;
	// if (Input::IsKeyPressed('D')) moveRight += 1.0f;
	// if (Input::IsKeyPressed('A')) moveRight -= 1.0f;
	// if (Input::IsKeyPressed('E')) moveUp += 1.0f;
	// if (Input::IsKeyPressed('Q')) moveUp -= 1.0f;

	const XMFLOAT3 forwardXZ = { std::sin(g_OniYaw), 0.0f, std::cos(g_OniYaw) };
	const XMFLOAT3 rightXZ = { std::cos(g_OniYaw), 0.0f, -std::sin(g_OniYaw) };

	XMFLOAT3 dir = {
		forwardXZ.x * moveForward + rightXZ.x * moveRight,
		moveUp,
		forwardXZ.z * moveForward + rightXZ.z * moveRight
	};

	const float lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	if (lenSq > 1.0f)
	{
		const float invLen = 1.0f / std::sqrt(lenSq);
		dir.x *= invLen;
		dir.y *= invLen;
		dir.z *= invLen;
	}

	const XMFLOAT3 velocity = {
		dir.x * kMoveSpeed,
		dir.y * kVerticalSpeed,
		dir.z * kMoveSpeed
	};

	auto* rigidbody = oni->GetComponent<CapsuleRigidbodyComponent>();
	if (rigidbody)
	{
		// TODO: CapsuleRigidbodyComponent の速度設定関数に合わせて変更してください
		// rigidbody->SetVelocity(velocity);
		oni->transform.position.x += velocity.x * dt;
		oni->transform.position.y += velocity.y * dt;
		oni->transform.position.z += velocity.z * dt;
	}
	else
	{
		oni->transform.position.x += velocity.x * dt;
		oni->transform.position.y += velocity.y * dt;
		oni->transform.position.z += velocity.z * dt;
	}

	// 一人称カメラ位置・視線更新
	const float cosPitch = std::cos(g_OniPitch);
	const XMFLOAT3 lookDir = {
		std::sin(g_OniYaw) * cosPitch,
		std::sin(g_OniPitch),
		std::cos(g_OniYaw) * cosPitch
	};

	const XMFLOAT3 basePos = oni->transform.position;
	const XMFLOAT3 eyePos = {
		basePos.x + lookDir.x * g_OniZoomOffset,
		basePos.y + g_OniEyeHeight + lookDir.y * g_OniZoomOffset,
		basePos.z + lookDir.z * g_OniZoomOffset
	};

	if (cameraObj)
	{
		cameraObj->transform.position = eyePos;
	}

	Camera& camera3D = Image::GetCamera3D();
	camera3D.position = eyePos;
	camera3D.target = {
		eyePos.x + lookDir.x,
		eyePos.y + lookDir.y,
		eyePos.z + lookDir.z
	};
	camera3D.up = { 0.0f, 1.0f, 0.0f };
}


bool CanOniSeePlayer(const GameObject* oni, const GameObject* player)
{
	if (!oni || !player)
	{
		return false;
	}

	// 鬼の目線位置とプレイヤーの中心付近（少し高め）を基準にする
	const XMFLOAT3 oniPos = oni->transform.position;
	const XMFLOAT3 eyePos = {
		oniPos.x,
		oniPos.y + g_OniEyeHeight,
		oniPos.z
	};

	const XMFLOAT3 playerPos = player->transform.position;
	const XMFLOAT3 targetPos = {
		playerPos.x,
		playerPos.y + 1.0f, // 足元ではなく胴体〜頭付近を狙う
		playerPos.z
	};

	// 1. 鬼 → プレイヤーのベクトルと距離を計算
	const float dx = targetPos.x - eyePos.x;
	const float dy = targetPos.y - eyePos.y;
	const float dz = targetPos.z - eyePos.z;
	const float distSq = dx * dx + dy * dy + dz * dz;

	if (distSq <= 0.0001f || distSq > kVisionRange * kVisionRange)
	{
		return false;
	}

	const float dist = std::sqrt(distSq);
	const XMFLOAT3 toPlayer = { dx / dist, dy / dist, dz / dist };

	// 2. 鬼の現在の視線ベクトル（Yaw / Pitch から算出）
	const float cosPitch = std::cos(g_OniPitch);
	const XMFLOAT3 lookDir = {
		std::sin(g_OniYaw) * cosPitch,
		std::sin(g_OniPitch),
		std::cos(g_OniYaw) * cosPitch
	};

	// 3. 内積で視野角内かチェック（cos(半角) 以上なら視野内）
	const float dot =
		lookDir.x * toPlayer.x +
		lookDir.y * toPlayer.y +
		lookDir.z * toPlayer.z;

	const float halfFovRad = XMConvertToRadians(kVisionFovDeg * 0.5f);
	const float cosHalfFov = std::cos(halfFovRad);

	if (dot < cosHalfFov)
	{
		return false;
	}

	// TODO: 壁や障害物による遮蔽判定を入れる場合はここにレイキャスト等を追加
	// 例: if (IsBlockedByObstacle(eyePos, targetPos)) return false;

	return true;
}