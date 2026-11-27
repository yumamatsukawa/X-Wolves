#pragma once
#include "../Engine/Core/Component.h"
#include <DirectXMath.h>

// 鬼の視点操作・移動・拡大縮小と、画面中央に映っている物の判定を行う
class OniController : public Component
{
private:
	static constexpr float MOVE_SPEED = 8.0f;            // 水平方向の移動速度
	static constexpr float MIN_ZOOM_HEIGHT = 8.0f;       // 一番拡大した時の高さ（仮の値）
	static constexpr float MAX_ZOOM_HEIGHT = 30.0f;      // 一番縮小した時の高さ（仮の値）
	static constexpr float ZOOM_STEP = 2.0f;             // ホイール1ノッチで視線方向に進む距離（仮の値）
	static constexpr float ZOOM_SPEED = 8.0f;            // 目標の位置に近づく速さ（仮の値）
	static constexpr float MOUSE_SENSITIVITY = 0.003f;   // マウス感度
	static constexpr float PITCH_LIMIT = DirectX::XM_PIDIV2 - 0.05f;  // 上下の向きの限界（真上・真下の手前）
	static constexpr float LOOK_RANGE = 200.0f;          // 画面中央の判定が届く距離（仮の値）

	float yaw = 0.0f;                            // 左右の向き（ラジアン。0で+Z、増えると右）
	float pitch = -DirectX::XM_PIDIV4;           // 上下の向き（ラジアン。負で下向き。最初は斜め下45度）
	DirectX::XMFLOAT3 zoomRemaining = {};        // 拡大縮小で、これから進む分
	DirectX::XMFLOAT3 zoomVelocity = {};         // 前のフレームに出した拡大縮小の速度

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;

	// 視線の向き
	DirectX::XMFLOAT3 GetLookDirection() const;
	// 画面中央に映っているオブジェクト（無ければnullptr。鬼自身は除く）
	// 普通の当たり判定で止まる。トリガーは通り抜けるが、タグがPlayer/NPC/UniqueNPCなら手前の物を返す
	GameObject* GetLookTarget() const;
	// 画面中央に映っているのがプレイヤーか（タグで判定）
	bool IsLookingAtPlayer() const;
};
