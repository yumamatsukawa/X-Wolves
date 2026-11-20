#pragma once
#pragma once

class Scene;
class GameObject;
enum class UniqueNpcType
{

    ZombieDog,//ゾンビ犬
    Witch,//魔女（通常の魔女とは色違いを想定）
    Vampire,//ドラキュラ
    Reaper,//死神（通常の死神とは色違いを想定）
    Werewolf,//狼男
    Necromancer,//ネクロマンサー（色違いの魔女を想定）
    CryingChild,//泣いている子供（泣き止んだ子供）
    GhostA,//幽霊A（ゴーストの色違いを想定）
    GhostB,//幽霊B（ゴーストの色違いを想定）
    CursedDoll,//呪いの人形（呪いの人形（カツラなし））"
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
GameObject* CreateUniqueNpc(Scene& scene, UniqueNpcType type, XMFLOAT3 pos);