#include "MissionComponent.h"
#include "../Engine/Physics/Physics.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"

void MissionComponent::Init()
{
	switch (missionID)
	{
	case 0:
		missionConditions.keyID = KEY_E;
		break;
	default:
		break;
	}
}

void MissionComponent::Update(float dt)
{
	bool nearby = Physics::OverlapSphere(GetOwner()->transform.position, 10.0f, "Player").empty();
	if (nearby) return;

	if (Input::GetKeyDown(missionConditions.keyID)) {
		// ミッションCLEARのイベントバス発行
	}
}