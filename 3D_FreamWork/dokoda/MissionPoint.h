#pragma once
#include "../Engine/Core/Transform.h"

class Scene;
class GameObject;
// 座標と半径は Missions.csv(PosX/PosY/PosZ/Radius)から読む
GameObject* CreateMissionPoint(Scene& scene, int missionId);