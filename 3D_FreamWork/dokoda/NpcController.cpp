#include "NpcController.h"
#include "NPC.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Core/MathUtil.h"
#include"../Engine/Behavior/PathFollowComponent.h"
struct NpcData
{
	GameObject* object = nullptr;

	NpcType Npctype;

	// 現在の行動
	MoveType Movetype;

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

    NavGridComponent* nav = nullptr;

	// 特殊アクションエリア内か
	bool IsSpecialArea = false;

    //ダンス中か
    bool isDance=false;

    //走っているか
    bool isDash=false;

	// 待機時間のタイマー
    float waitTimer=0;

   
  
};
float NpcWalkSpeed = 10;//歩くスピード
float NpcDashSpeed = 20;//ダッシュスピード
NpcPersonality MoveData[static_cast<int>(ActionType::Count)]//性格による各確率の変化
{
    // Stop
    {
        ActionType::Stop,
        {
            {MoveType::Move, 0},
            {MoveType::Wait, 100},
            {MoveType::SpecialAction, 0},
            {MoveType::Dance, 0}
        }
    },
    // Move
    {
        ActionType::Move,
        {
            {MoveType::Move, 50},
            {MoveType::Wait, 0},
            {MoveType::SpecialAction, 0},
            {MoveType::Dance, 0}
        }
    },
    // Motion
    {
        ActionType::Motion,
        {
            {MoveType::Move, 30},
            {MoveType::Wait, 30},
            {MoveType::SpecialAction, 10},
            {MoveType::Dance, 10}
        }
    },
    // Attack
    {
        ActionType::Attack,
        {
            {MoveType::Move, 20},
            {MoveType::Wait, 20},
            {MoveType::SpecialAction, 30},
            {MoveType::Dance, 20}
        }
    }
};
NpcData* NPC[NPC_MAX];
Event event;
int npcCount = 0;
void NpcController::Init()
{
 
    for (int i = 0; i < static_cast<int>(NpcType::Count); i++)
    {
        for (int j = 0; j < NPC_MAX / static_cast<int>(NpcType::Count); j++)
        {
            NpcType type = static_cast<NpcType>(i);
            XMFLOAT3 pos;

            //行動できるエリアの受け取るプログラムが必要
            NPC[npcCount]->ActionData->MINMove.x;
            NPC[npcCount]->ActionData->MAXMove.x;

          
            while (true)
            {
                float x = Math::RandomRange(NPC[npcCount]->ActionData->MINMove.x, NPC[npcCount]->ActionData->MAXMove.x);
                float z = Math::RandomRange(NPC[npcCount]->ActionData->MINMove.z, NPC[npcCount]->ActionData->MAXMove.z);
                //ここに建物の位置とサイズを取得する処理を追加する必要があります。

            // 目的地が建物と重なっていないかチェック
                if (CheckDestination({ x, 0.0f, z }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }))
                {
                    pos = { x, 0.0f, z };
                }
            }
			NPC[npcCount]->object = CreateNpc(*GetOwner()->GetScene(), type, pos);
            NPC[npcCount]->Npctype = type;
            NPC[npcCount]->Destination = { 0.0f, 0.0f, 0.0f };
            NPC[npcCount]->Movetype = MoveType::Wait;
            NPC[npcCount]->Path = NPC[npcCount]->object->AddComponent<PathFollowComponent>();
            NPC[npcCount]->Path->SetPlane(MovePlane::XZ);
            NPC[npcCount]->Path->SetNavigation(NPC[npcCount]->nav);
            NPC[npcCount]->Path->SetSpeed(NPC[npcCount]->Speed);
			
			npcCount++;
               
            
        }
    }
}
void NpcController::Update(float dt)
{
    if (Time::IsPaused()) return;

    //イベントが発生したか受け取るプログラム

    for (int NPC_No = 0; NPC_No < npcCount; NPC_No++)
    {
        
        if (NPC[NPC_No]->IsMoving)
        {
            NpcSetDestination(NPC_No);
        }
        else
        {
            switch (NPC[NPC_No]->Movetype)
            {
            case MoveType::Move:
                // 目的地に向かって移動
                NpcMove(NPC_No, dt);
                break;

            case MoveType::Wait:
                // 一定時間待機
                NpcWait(NPC_No, dt);
                break;

            case MoveType::SpecialAction:
                // 特殊行動
                NpcSpecialAction(NPC_No, dt);
                break;

            case MoveType::Dance:
                // ダンス
                NpcDance(NPC_No, dt);
                break;
            }
        }
    }
}
void NpcController::OnCollisionEnter(CollisionInfo info)
{
    if (info.other == nullptr) return;

    if (info.other->GetTag() == "NPC" ||
        info.other->GetTag() == "Player")
    {
      // 衝突した相手がNPCまたはPlayerの場合の処理
		// ここに衝突時の処理を追加することができます
    }
}
void NpcSetDestination(int NPC_No)
{
    int random = Math::RandomRange(0, 1);
    if (random==0)
    {
        NPC[NPC_No]->Speed = NpcWalkSpeed;
    }
    else
    {
        NPC[NPC_No]->Speed = NpcDashSpeed;

    }
    if (NPC[NPC_No]->Movetype==MoveType::SpecialAction)
    {
		// 特殊行動の目的地の座標の獲得

        NPC[NPC_No]->Destination;//= 目的地の座標;
        NPC[NPC_No]->Path->SetTargetPosition(
            NPC[NPC_No]->Destination
        );

        NPC[NPC_No]->IsMoving = false;
        NPC[NPC_No]->Movetype = MoveType::Move;
    }
    else if (NPC[NPC_No]->Movetype == MoveType::Move)
    {
        float x = Math::RandomRange(NPC[NPC_No]->ActionData->MINMove.x, NPC[NPC_No]->ActionData->MAXMove.x);
        float z = Math::RandomRange(NPC[NPC_No]->ActionData->MINMove.z, NPC[NPC_No]->ActionData->MAXMove.z);
        //ここに建物の位置とサイズを取得する処理を追加する必要があります。

    // 目的地が建物と重なっていないかチェック
        if (CheckDestination({ x, 0.0f, z }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }))
        {
            NPC[NPC_No]->Destination = { x, 0.0f, z };
            NPC[NPC_No]->Path->SetTargetPosition(
                NPC[NPC_No]->Destination
            );

            NPC[NPC_No]->IsMoving = false;
            NPC[NPC_No]->Movetype = MoveType::Move;
        }
    }
   
	
}
void NpcWait(int NPC_No, float dt)
{
    
    // 経過時間を加算
    NPC[NPC_No]->waitTimer += dt;

    // 3秒経過したら待機終了
    if (NPC[NPC_No]->waitTimer >= 3.0f)
    {
        int personalityIndex = static_cast<int>(ActionType::Motion);

        const auto& personality = MoveData[personalityIndex];

        int random = Math::RandomRange(0, 99);
        int totalProbability = 0;

        for (const auto& action : personality.Probabilities)
        {
            totalProbability += action.probability;

            if (random < totalProbability)
            {
                switch (action.MoveType)
                {
                    case MoveType::Move:
                        NPC[NPC_No]->IsMoving = true;
                    break;
                    case MoveType::Wait:
                    break;
                    case MoveType::SpecialAction:
                        NPC[NPC_No]->IsSpecialArea = true;
                        NPC[NPC_No]->IsMoving = true;
                    break;
                    case MoveType::Dance:
					break;
                default:
                    break;
                }
				NPC[NPC_No]->waitTimer = 0.0f; // タイマーをリセット
                NPC[NPC_No]->Movetype = action.MoveType;
                break;
            }
        }
    }
}
void NpcMove(int NPC_No, float dt)
{
    // 経路が見つかっているか確認
    if (!NPC[NPC_No]->Path->HasPath())
    {
        return;
    }

    // 目的地に到着したか確認
    if (NPC[NPC_No]->Path->HasArrived())
    {
        if (NPC[NPC_No]->IsSpecialArea)
        {
            NPC[NPC_No]->Movetype = MoveType::SpecialAction;
        }
        else
        {

            int personalityIndex = static_cast<int>(ActionType::Motion);

            const auto& personality = MoveData[personalityIndex];

            int random = Math::RandomRange(0, 99);
            int totalProbability = 0;

            for (const auto& action : personality.Probabilities)
            {
                totalProbability += action.probability;

                if (random < totalProbability)
                {
                    switch (action.MoveType)
                    {
                    case MoveType::Move:
                        NPC[NPC_No]->IsMoving = true;
                        break;
                    case MoveType::Wait:
                        break;
                    case MoveType::SpecialAction:
                        NPC[NPC_No]->IsSpecialArea = true;
                        NPC[NPC_No]->IsMoving = true;
                        break;
                    case MoveType::Dance:
                        break;
                    default:
                        break;
                    }
                    NPC[NPC_No]->waitTimer = 0.0f; // タイマーをリセット
                    NPC[NPC_No]->Movetype = action.MoveType;
                    break;
                }
            }
        }
    }
}
void NpcSpecialAction(int NPC_No, float dt)
{
    // 経過時間を加算
    NPC[NPC_No]->waitTimer += dt;

    //特殊モーションを流すプログラム

    // 3秒経過したら待機終了
    if (NPC[NPC_No]->waitTimer >= 3.0f)
    {
        int personalityIndex = static_cast<int>(ActionType::Motion);

        const auto& personality = MoveData[personalityIndex];

        int random = Math::RandomRange(0, 99);
        int totalProbability = 0;

        for (const auto& action : personality.Probabilities)
        {
            
            totalProbability += action.probability;

            if (random < totalProbability)
            {
                NPC[NPC_No]->waitTimer = 0.0f; // タイマーをリセット
                NPC[NPC_No]->IsSpecialArea = false;
                switch (action.MoveType)
                {
                case MoveType::Move:
                    NPC[NPC_No]->IsMoving = true;
                    break;
                case MoveType::Wait:
                    break;
                case MoveType::SpecialAction:
                    NPC[NPC_No]->IsSpecialArea = true;
                    NPC[NPC_No]->IsMoving = true;
                    break;
                case MoveType::Dance:
                    break;
                default:
                    break;
                }
                
                NPC[NPC_No]->Movetype = action.MoveType;
                break;
            }
        }
    }
}
void NpcDance(int NPC_No, float dt)
{

    // 経過時間を加算
    NPC[NPC_No]->waitTimer += dt;

    //ダンスモーションを流すプログラム

    // 3秒経過したら待機終了
    if (NPC[NPC_No]->waitTimer >= 3.0f)
    {
        int personalityIndex = static_cast<int>(ActionType::Motion);

        const auto& personality = MoveData[personalityIndex];

        int random = Math::RandomRange(0, 99);
        int totalProbability = 0;

        for (const auto& action : personality.Probabilities)
        {

            totalProbability += action.probability;

            if (random < totalProbability)
            {
                NPC[NPC_No]->waitTimer = 0.0f; // タイマーをリセット
                switch (action.MoveType)
                {
                case MoveType::Move:
                    NPC[NPC_No]->IsMoving = true;
                    break;
                case MoveType::Wait:
                    break;
                case MoveType::SpecialAction:
                    NPC[NPC_No]->IsSpecialArea = true;
                    NPC[NPC_No]->IsMoving = true;
                    break;
                case MoveType::Dance:
                    break;
                default:
                    break;
                }

                break;
            }
        }
    }
}
void NpcEvent(int NPC_No, float dt)
{
    switch (event)
    {
    case Event::Dance:
    {
        for (int NPC_No = 0; NPC_No < npcCount; NPC_No++)
        {
            NPC[NPC_No]->Movetype = MoveType::Dance;
        }
        break;
    }

    case Event::Jukebox:
    {
        // ジュークボックスの座標を受け取る
        XMFLOAT3 jukebox;

        // ジュークボックスに向かえるNPCを探す
        std::vector<int> candidates;

        for (int NPC_No = 0; NPC_No < npcCount; NPC_No++)
        {
            // 待機中のNPCだけ対象
            if (NPC[NPC_No]->Movetype != MoveType::Wait)
                continue;

            // 行動範囲内か確認
            // NPCの現在位置とジュークボックスの距離を確認する

            candidates.push_back(NPC_No);
        }

        // 候補が存在する場合
        if (!candidates.empty())
        {
            // 候補からランダムに1人選択
            int randomIndex = rand() % candidates.size();
            int selectedNPC = candidates[randomIndex];

            // 選ばれたNPCだけジュークボックスへ移動
            NPC[selectedNPC]->Destination = jukebox;
            NPC[selectedNPC]->Movetype = MoveType::SpecialAction;
            NPC[selectedNPC]->IsMoving = true;
        }

        break;
    }

    case Event::Switch:
    {
        // スイッチの座標を受け取る
        XMFLOAT3 Switch;

        // スイッチに向かえるNPCを探す
        std::vector<int> candidates;

        for (int NPC_No = 0; NPC_No < npcCount; NPC_No++)
        {
            // 行動範囲内か確認
            // NPCの現在位置とスイッチの距離を確認する

            candidates.push_back(NPC_No);
        }

        // 候補が存在する場合
        if (!candidates.empty())
        {
            // 候補からランダムに1人選択
            int randomIndex = rand() % candidates.size();
            int selectedNPC = candidates[randomIndex];

            // 選ばれたNPCだけスイッチへ移動
            NPC[selectedNPC]->Destination = Switch;
            NPC[selectedNPC]->Movetype = MoveType::SpecialAction;
            NPC[selectedNPC]->IsMoving = true;
        }

        break;
    }

    default:
        break;
    }
}
bool CheckDestination(
    const XMFLOAT3& destination,
    const XMFLOAT3& buildingPosition,
    const XMFLOAT3& buildingSize)
{
    float halfWidth = buildingSize.x / 2.0f;
    float halfHeight = buildingSize.y / 2.0f;

    if (destination.x >= buildingPosition.x - halfWidth &&
        destination.x <= buildingPosition.x + halfWidth &&
        destination.y >= buildingPosition.y - halfHeight &&
        destination.y <= buildingPosition.y + halfHeight)
    {
        return false; // 建物と重なっている
    }

    return true; // 移動可能
}

