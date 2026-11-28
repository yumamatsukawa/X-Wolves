#pragma once

#include"../Engine/Core/Component.h"
#include"../Engine/Render3D/LightComponent.h"
#include <DirectXMath.h>

class LightController : public Component 
{
private:
	LightComponent* light = nullptr;

	//一秒間の回転角度
	float rotateSpeed = 90.0f;

	//上下方向の制限
	float minPitch = -80.0f;
	float maxPitch = 80.0f;

	//鬼を照らしているか
	bool hitOni = false;
	//目くらまし状態
	bool oniBlind = false;
	//目くらまし時間
	float blindTimer = 0.0f;
	//目くらまし経過時間
	float blindDuration = 3.0f;

	//rotateの向きをライトに反映する
	void ApplyDirection();

public:
	void Init() override;
	void Update(float dt) override;

	//鬼に光が当たった時
	void HitOni();

	bool IsoniBlind() const
	{
		return oniBlind;
	}

};

