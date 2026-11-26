#pragma once
#include "../Engine/Core/Component.h"

class PlayerController : public Component {
private:
	float speed = 20.0f;

public:
	void Init() override;
	void Update(float dt) override;

};