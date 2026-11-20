#pragma once

class Scene;
class GameObject;
enum class NpcType
{
    Zombie,//ゾンビ
    Vampire,//ヴァンパイア
    Ghost,//ゴースト
    Witch,//魔女
    Reaper,//死神

	Count
};
enum class ColorType
{
    Red,
    Blue,
    Green,
    Yellow,

    Count
};
enum class MoveType
{
    Move,            // 移動
    Wait,            // 待機
    SpecialAction,   // 特殊行動
    Dance,           // ダンス
    Count
};
enum class ActionType
{
    Stop,
    Move,
    Motion,
    Attack,

    Count
};
struct ActionProbability	//行動確率を保持する変数
{
	MoveType MoveType;// NPCの行動タイプを保持する変数
	int probability;// NPCの行動確率を保持する変数
};
struct NpcPersonality//NPCの性格を保持する変数
{
	ActionType ActionType; // NPCの行動タイプを保持する変数
	ActionProbability Probabilities[static_cast<int>(MoveType::Count)];// NPCの行動確率を保持する配列
};
struct NpcAction
{
	NpcPersonality NpcPersonality; //NPCの性格を保持する変数
    XMFLOAT3 MAXMove;//NPCの移動範囲の最大値を保持する変数
    XMFLOAT3 MINMove;//NPCの移動範囲の最小値を保持する変数
};
GameObject* CreateNpc(Scene& scene, NpcType type, XMFLOAT3 pos);