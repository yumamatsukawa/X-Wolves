#include "NpcController.h"
#include"Map.h"
#include"MissionPoint.h"
#include"../Engine/Core/Scene.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Core/MathUtil.h"
#include"../Engine/Behavior/PathFollowComponent.h"
#include"../Engine/Core/EventBus.h"

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

void NpcController::Init()
{
   
        // ダンスイベント
        EventBus::Get().Subscribe("Dance", [this]() {
            OnDanceEvent();
            });

        // ジュークボックスイベント
        EventBus::Get().Subscribe("Jukebox", [this]() {
            OnJukeboxEvent();
            });

        // スイッチイベント
        EventBus::Get().Subscribe("Switch", [this]() {
            OnSwitchEvent();
            });
    
    for (int i = 0; i < static_cast<int>(NpcType::Count); i++)
    {
        for (int j = 0; j < NPC_MAX / static_cast<int>(NpcType::Count); j++)
        {
            NpcType type = static_cast<NpcType>(i);
            NPC[npcCount] = new NpcData();
            XMFLOAT3 pos;

            //行動できるエリアの受け取るプログラムが必要
            NPC[npcCount]->ActionData->MINMove.x=0;
            NPC[npcCount]->ActionData->MAXMove.x=100;

          
            while (true)
            {
                float x = Math::RandomRange(NPC[npcCount]->ActionData->MINMove.x, NPC[npcCount]->ActionData->MAXMove.x);
                float z = Math::RandomRange(NPC[npcCount]->ActionData->MINMove.z, NPC[npcCount]->ActionData->MAXMove.z);
                //ここに建物の位置とサイズを取得する処理を追加する必要があります。

            // 目的地が建物と重なっていないかチェック
                if (CheckDestination({ x, 0.0f, z }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }))
                {
                    pos = { x, 0.0f, z };
                    break;
                }
            }
			NPC[npcCount]->object = CreateNpc(*GetOwner()->GetScene(), type, pos);
            NPC[npcCount]->Npctype = type;
            NPC[npcCount]->Destination = { 0.0f, 0.0f, 0.0f };
            NPC[npcCount]->Movetype = MoveType::Wait;
            NPC[npcCount]->Path = NPC[npcCount]->object->AddComponent<PathFollowComponent>();
            NPC[npcCount]->Path->SetPlane(MovePlane::XZ);
            NPC[npcCount]->Path->SetNavigation(nav);
            NPC[npcCount]->Path->SetSpeed(NpcWalkSpeed);

			npcCount++;
               
            
        }
    }
}
void NpcController::Update(float dt)
{
    if (Time::IsPaused()) return;


    for (int NPC_No = 0; NPC_No < npcCount; NPC_No++)
    {
        //着地確認
        NPC[NPC_No]->IsGrounded = CheckGrounded(NPC[NPC_No]->object);

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
                NpcJump(NPC_No, dt);
                break;

            case MoveType::Wait:
                // 一定時間待機
                NpcWait(NPC_No, dt);
                NpcJump(NPC_No, dt);
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
void NpcController::NpcSetDestination(int NPC_No)
{
    int random = Math::RandomRange(0, 1);
    if (random==0)
    {
        NPC[npcCount]->Path->SetSpeed(NpcWalkSpeed);
    }
    else
    {
        NPC[npcCount]->Path->SetSpeed(NpcDashSpeed);
    }
    random = Math::RandomRange(0, 1);

    if (NPC[NPC_No]->Movetype == MoveType::SpecialAction)
    {
        // ポイントが存在するか確認
        if (MissionPoints.empty())
        {
            return;
        }

        // ランダムにポイントを選択
        int index = Math::RandomRange(
            0,
            static_cast<int>(MissionPoints.size()) - 1
        );

        // 選択したポイントの座標を取得
        XMFLOAT3 pos = MissionPoints[index]->transform.position;

        // NPCの目的地に設定
        NPC[NPC_No]->Destination = pos;

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
        Scene* scene = GetOwner()->GetScene();

        GameObject* Map = CreateGround(*scene);

    // 目的地が建物と重なっていないかチェック
        if (CheckDestination({ x, 0.0f, z }, Map->transform.position, Map->transform.scale))
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
void NpcController::NpcWait(int NPC_No, float dt)
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
void NpcController::NpcMove(int NPC_No, float dt)
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
void NpcController::NpcSpecialAction(int NPC_No, float dt)
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
void NpcController::NpcDance(int NPC_No, float dt)
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
void NpcController::OnDanceEvent()
{
    for (int i = 0; i < npcCount; i++)
    {
        NPC[i]->Movetype = MoveType::Dance;
    }
}
void NpcController::OnJukeboxEvent()
{
    XMFLOAT3 jukebox= { 0,0,0 };

    //ジュークボックスの位置を受け取る

    std::vector<int> candidates;

    for (int i = 0; i < npcCount; i++)
    {
        // 待機中のNPCだけ対象
        if (NPC[i]->Movetype != MoveType::Wait)
            continue;

        // 候補に追加
        candidates.push_back(i);
    }

    // 候補が存在する場合
    if (!candidates.empty())
    {
        int randomIndex = rand() % candidates.size();
        int selectedNPC = candidates[randomIndex];

        NPC[selectedNPC]->Destination = jukebox;
        NPC[selectedNPC]->Movetype = MoveType::SpecialAction;
        NPC[selectedNPC]->IsMoving = true;
    }
}
void NpcController::OnSwitchEvent()
{
    XMFLOAT3 switchPosition = { 0,0,0 };

    //スイッチの位置を受け取る

    std::vector<int> candidates;

    for (int i = 0; i < npcCount; i++)
    {
        candidates.push_back(i);
    }

    if (!candidates.empty())
    {
        int randomIndex = rand() % candidates.size();
        int selectedNPC = candidates[randomIndex];

        NPC[selectedNPC]->Destination = switchPosition;
        NPC[selectedNPC]->Movetype = MoveType::SpecialAction;
        NPC[selectedNPC]->IsMoving = true;
    }
}
bool NpcController::CheckDestination(
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
void NpcController::Setup(NavGridComponent* navigation)
{
      nav = navigation;
}
void NpcController::NpcJump(int NPC_No, float dt)
{
    // 地面にいない場合はジャンプできない
    if (!NPC[NPC_No]->IsGrounded)
    {
        return;
    }

    // Rigidbodyを取得
    RigidbodyComponent* rb =
        NPC[NPC_No]->object->GetComponent<RigidbodyComponent>();

    if (rb == nullptr)
    {
        return;
    }

    // 抽選までの時間を減らす
    NPC[NPC_No]->JumpTimer -= dt;

    if (NPC[NPC_No]->JumpTimer > 0.0f)
    {
        return;
    }

    // 次の抽選まで3～8秒待つ
    NPC[NPC_No]->JumpTimer = Math::RandomRange(3.0f, 8.0f);

    // 30%の確率でジャンプ
    int random = Math::RandomRange(0, 99);

    if (random < 30)
    {
        // 上方向にジャンプ
        rb->SetVelocity({
            0.0f,
            NPC[NPC_No]->JumpPower,
            0.0f
            });

        NPC[NPC_No]->IsGrounded = false;
        NPC[NPC_No]->IsJumping = true;
    }
}
bool NpcController::CheckGrounded(GameObject* npc)
{
    // NPCの現在位置
    XMFLOAT3 position = npc->transform.position;

    // NPCの足元から少し下にずらす
    XMFLOAT3 origin = {
        position.x,
        position.y - 0.5f,
        position.z
    };

    // 下方向
    XMFLOAT3 direction = { 0.0f, -1.0f, 0.0f };

    // 判定結果を受け取る変数
    RaycastHit hit;

    // 地面との衝突判定
    if (Physics::Raycast(origin, direction, 0.2f,
        hit, "Ground", QueryTriggers::Ignore))
    {
        return true;
    }

    return false;
}