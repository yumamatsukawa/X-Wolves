#pragma once
#include "../Engine/Core/Component.h"
#include "NPC.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Core/MathUtil.h"
#include"../Engine/Behavior/PathFollowComponent.h"
#include"../Engine/Core/EventBus.h"
#define NPC_MAX (10)
class NavGridComponent;
class NpcController : public Component {
private:
	struct NpcData
	{
		GameObject* object = nullptr;

		NpcType Npctype= NpcType::Ghost;

		// 現在の行動
		MoveType Movetype= MoveType::Dance;

		// 移動先
		XMFLOAT3 Destination;
		bool IsMoving = false;

		//行動に関するデータ
		NpcAction* ActionData;

		// 待機時間
		float WaitTime = 0.0f;

		// 経過時間
		float Timer = 0.0f;

		// 移動速度
		float Speed = 0.0f;

		//
		PathFollowComponent* Path = nullptr;


		// 特殊アクションエリア内か
		bool IsSpecialArea = false;

		//ダンス中か
		bool isDance = false;

		//走っているか
		bool isDash = false;

		// 待機時間のタイマー
		float waitTimer = 0;

		bool IsGrounded = false;  // 地面に接触しているか
		bool IsJumping = false;   // ジャンプ中か
		float JumpPower = 10.0f;  // ジャンプ力
		float JumpTimer = 0.0f;       // 次の抽選までの時間
		float JumpElapsed = 0.0f;     // ジャンプ経過時間

	};
	float NpcWalkSpeed = 10;//歩くスピード
	float NpcDashSpeed = 20;//ダッシュスピード
	NpcData* NPC[NPC_MAX];
	NavGridComponent* nav = nullptr;
	int npcCount = 0;
public:
	void Init() override;
	void Update(float dt) override;
	void OnDanceEvent();
	void OnJukeboxEvent();
	void OnSwitchEvent();
	void NpcSetDestination(int NPC_No);
	void NpcWait(int NPC_No, float dt);
	void NpcMove(int NPC_No, float dt);
	void NpcSpecialAction(int NPC_No, float dt);
	void NpcDance(int NPC_No, float dt);
	bool CheckDestination(
		const XMFLOAT3& destination,
		const XMFLOAT3& buildingPosition,
		const XMFLOAT3& buildingSize);
	void Setup(NavGridComponent* navigation);
	void NpcJump(int NPC_No, float dt);
	bool CheckGrounded(GameObject* npc);
};
enum class Event
{
	Dance,// ダンス
	Jukebox,//
	Switch,//
	Count
};

extern std::vector<GameObject*> MissionPoints;
