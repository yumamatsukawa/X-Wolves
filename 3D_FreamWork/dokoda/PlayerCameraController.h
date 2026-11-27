#pragma once
#include "../Engine/Core/Component.h"
#include <DirectXMath.h>

// プレイヤーを中心に、マウスで回る三人称視点(TPS)カメラ
class PlayerCameraController : public Component
{
private:
	static constexpr float CAMERA_DISTANCE = 10.0f;      // 注視点からカメラまでの距離（仮の値。nearZ=5より大きくする）
	static constexpr float TARGET_HEIGHT = 1.0f;         // 注視点の高さ。プレイヤーの中心から上へ（仮の値）
	static constexpr float START_PITCH = -DirectX::XM_PI / 6.0f;  // 開始時の上下の向き。斜め下30度（仮の値）
	static constexpr float MIN_PITCH = -1.4f;            // 一番見下ろす向き。真上の手前（仮の値）
	static constexpr float MAX_PITCH = -0.25f;            // 一番水平に近い向き。地面に潜らない（仮の値）
	static constexpr float MOUSE_SENSITIVITY = 0.003f;   // マウス感度（仮の値）

	float yaw = 0.0f;             // 左右の向き（ラジアン。0で+Z、増えると右）
	float pitch = START_PITCH;    // 上下の向き（ラジアン。負で下向き）

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;

	// カメラの左右の向き（ラジアン。0で+Z、増えると右）
	float GetYaw() const { return yaw; }
};
