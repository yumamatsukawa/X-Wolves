#include "OniController.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Core/MathUtil.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Physics/Physics.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include <cmath>

namespace
{
	// すり抜ける当たり判定(トリガー)でも、画面中央の判定で返すタグ
	constexpr const char* TRIGGER_TARGET_TAGS[] = { "Player", "NPC", "UniqueNPC" };

	constexpr float SELF_SKIP_DISTANCE = 0.01f;  // 自分に当たった時に、中へ入り直す距離

	// selfを除いたRaycast。hit.distanceはいつもoriginからの距離
	bool RaycastExceptSelf(const XMFLOAT3& origin, const XMFLOAT3& dir, float range,
		const GameObject* self, const char* tag, QueryTriggers triggers, RaycastHit& hit)
	{
		if (!Physics::Raycast(origin, dir, range, hit, tag, triggers)) return false;
		if (hit.object != self) return true;

		// 移動直後はtransformが当たり判定より遅れていて自分に当たることがあるので、中に入ってから飛ばし直す
		// (始点が中に入っている物には当たらない)
		const XMFLOAT3 insidePoint = hit.point + dir * SELF_SKIP_DISTANCE;
		const float skipped = hit.distance + SELF_SKIP_DISTANCE;
		if (!Physics::Raycast(insidePoint, dir, range - skipped, hit, tag, triggers)) return false;
		hit.distance += skipped;
		return hit.object != self;
	}
}

void OniController::Init()
{
	// マウスで視点を動かすので、カーソルを固定する
	Input::SetCursorLocked(true);
}

void OniController::Uninit()
{
	Input::SetCursorLocked(false);
}

void OniController::Update(float dt)
{
	// ポーズ中・シーン切り替え中は操作を受け付けない(物理も止まるので、速度はそのままでよい)
	if (Time::IsPaused()) return;

	GameObject* oni = GetOwner(); if (!oni) return;

	// 視点操作: 固定が解除されていたら、左クリックで固定し直す
	if (Input::IsCursorLocked())
	{
		const XMFLOAT2 delta = Input::GetMouseDelta();
		yaw += delta.x * MOUSE_SENSITIVITY;
		pitch -= delta.y * MOUSE_SENSITIVITY;
		pitch = Math::Clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT);
	}
	else if (Input::GetKeyDown(MOUSE_LEFT))
	{
		Input::SetCursorLocked(true);
	}

	XMFLOAT2 vel{};

	if (Input::GetKeyPress(KEY_W)) vel.y += 1.0f;
	if (Input::GetKeyPress(KEY_S)) vel.y -= 1.0f;
	if (Input::GetKeyPress(KEY_D)) vel.x += 1.0f;
	if (Input::GetKeyPress(KEY_A)) vel.x -= 1.0f;

	const XMFLOAT3 forwardXZ = { std::sin(yaw), 0.0f, std::cos(yaw) };
	const XMFLOAT3 rightXZ = { std::cos(yaw), 0.0f, -std::sin(yaw) };

	XMFLOAT3 dir = {
		forwardXZ.x * vel.y + rightXZ.x * vel.x,
		0.0f,
		forwardXZ.z * vel.y + rightXZ.z * vel.x
	};

	// 斜め移動が速くならないようにする
	const float lenSq = dir.x * dir.x + dir.z * dir.z;
	if (lenSq > 1.0f)
	{
		const float invLen = 1.0f / std::sqrt(lenSq);
		dir.x *= invLen;
		dir.z *= invLen;
	}

	// 拡大縮小: 直前の物理で進んだ分を、残りから引く
	zoomRemaining = zoomRemaining - zoomVelocity * dt;

	// 目標の高さは範囲に収める(開始時に範囲の外にいる場合も、ここで範囲まで移動する)
	const float height = oni->transform.position.y;
	const float targetHeight = Math::Clamp(height + zoomRemaining.y, MIN_ZOOM_HEIGHT, MAX_ZOOM_HEIGHT);
	zoomRemaining.y = targetHeight - height;

	// ホイールを奥に回すと視線の方向へ進み、手前に回すと下がる
	XMFLOAT3 step = GetLookDirection() * (Input::GetMouseWheel() * ZOOM_STEP);

	// 高さの上限・下限を超える時は、超えない所まで全体を縮める(横だけ進み続けないように)
	const float nextHeight = targetHeight + step.y;
	const float limitedHeight = Math::Clamp(nextHeight, MIN_ZOOM_HEIGHT, MAX_ZOOM_HEIGHT);
	if (limitedHeight != nextHeight) step = step * ((limitedHeight - targetHeight) / step.y);
	zoomRemaining = zoomRemaining + step;

	// 残りの距離に比例した速度で滑らかに近づける(1フレームで行き過ぎないようにする)
	const float zoomRate = (ZOOM_SPEED * dt > 1.0f) ? 1.0f / dt : ZOOM_SPEED;
	zoomVelocity = zoomRemaining * zoomRate;

	const XMFLOAT3 velocity = dir * MOVE_SPEED + zoomVelocity;

	auto* rigidbody = oni->GetComponent<RigidbodyComponent>();
	if (rigidbody) rigidbody->SetVelocity(velocity);
}

XMFLOAT3 OniController::GetLookDirection() const
{
	const float cosPitch = std::cos(pitch);
	return {
		std::sin(yaw) * cosPitch,
		std::sin(pitch),
		std::cos(yaw) * cosPitch
	};
}

GameObject* OniController::GetLookTarget() const
{
	const GameObject* oni = GetOwner(); if (!oni) return nullptr;

	// 鬼の中心(カメラの位置)から飛ばす。始点が中に入っている物には当たらないので、鬼自身は除かれる
	const XMFLOAT3 origin = oni->transform.position;
	const XMFLOAT3 lookDir = GetLookDirection();

	// 最初にぶつかる普通の当たり判定(無ければ、届く距離の限界まで調べる)
	RaycastHit nearest;
	if (!RaycastExceptSelf(origin, lookDir, LOOK_RANGE, oni, "", QueryTriggers::Ignore, nearest))
	{
		nearest = RaycastHit();
		nearest.distance = LOOK_RANGE;
	}

	// それより手前に対象のタグがあれば(トリガーでも)、いちばん近い物にする
	for (const char* tag : TRIGGER_TARGET_TAGS)
	{
		RaycastHit hit;
		if (RaycastExceptSelf(origin, lookDir, nearest.distance, oni, tag, QueryTriggers::Include, hit)
			&& hit.distance < nearest.distance)
		{
			nearest = hit;
		}
	}

	return nearest.object;
}

bool OniController::IsLookingAtPlayer() const
{
	const GameObject* target = GetLookTarget();
	return target && target->GetTag() == "Player";
}
