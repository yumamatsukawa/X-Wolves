#pragma once
#include "../Engine/Core/Component.h"

class GameObject;
class PlayerController : public Component {
private:
	float speed = 20.0f;
	GameObject* ground = nullptr;

public:
	void Init() override;
	void Update(float dt) override;

};