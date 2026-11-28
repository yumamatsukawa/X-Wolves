#include "PlayerCameraController.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Core/MathUtil.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Render/Image.h"
#include <cmath>

void PlayerCameraController::Init()
{
	Image::SetCamera3DAutoSync(false);
	// マウスで視点を動かすので、カーソルを固定する
	Input::SetCursorLocked(true);
}

void PlayerCameraController::Uninit()
{
	Input::SetCursorLocked(false);
	Image::SetCamera3DAutoSync(true);
}

void PlayerCameraController::Update(float dt)
{
	GameObject* player = GetOwner(); if (!player) return;

	// 視点操作(ポーズ中は受け付けない): 固定が解除されていたら、左クリックで固定し直す
	if (!Time::IsPaused())
	{
		if (Input::IsCursorLocked())
		{
			const XMFLOAT2 delta = Input::GetMouseDelta();
			yaw += delta.x * MOUSE_SENSITIVITY;
			pitch -= delta.y * MOUSE_SENSITIVITY;
			pitch = Math::Clamp(pitch, MIN_PITCH, MAX_PITCH);
		}
		else if (Input::GetKeyDown(MOUSE_LEFT))
		{
			Input::SetCursorLocked(true);
		}
	}

	// 視線の向き
	const float cosPitch = std::cos(pitch);
	const XMFLOAT3 lookDir = {
		std::sin(yaw) * cosPitch,
		std::sin(pitch),
		std::cos(yaw) * cosPitch
	};

	// プレイヤーの少し上を注視点にして、視線の逆向きに離れた位置から見る
	const XMFLOAT3 target = player->transform.position + XMFLOAT3{ 0.0f, TARGET_HEIGHT, 0.0f };

	// 上を向いて地面にめり込む時は、カメラを注視点に近づける
	float distance = CAMERA_DISTANCE;
	if (lookDir.y > 0.0f)
	{
		// カメラの高さ = target.y - lookDir.y * distance が下限になる距離
		const float maxDistance = (target.y - MIN_CAMERA_HEIGHT) / lookDir.y;
		if (distance > maxDistance) distance = maxDistance;
	}

	Camera& camera3D = Image::GetCamera3D();
	camera3D.position = target - lookDir * distance;
	camera3D.target = target;
	camera3D.up = { 0.0f, 1.0f, 0.0f };
}
