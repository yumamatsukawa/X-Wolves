#pragma once
#include "../Engine/Core/Component.h"
#define NPC_MAX (100)
class NpcController : public Component {
private:

public:
	void Init() override;
	void Update(float dt) override;
	void OnCollisionEnter(CollisionInfo info) override;
};
enum class Event
{
	Dance,// ƒ_ƒ“ƒX
	Jukebox,//
	Switch,//
	Count
};

void NpcSetDestination(int NPC_No);
void NpcWait(int NPC_No, float dt);
void NpcMove(int NPC_No, float dt);
void NpcSpecialAction(int NPC_No, float dt);
void NpcDance(int NPC_No, float dt);
bool CheckDestination(
	const XMFLOAT3& destination,
	const XMFLOAT3& buildingPosition,
	const XMFLOAT3& buildingSize);