#pragma once
#include "../Engine/Core/Transform.h"

class Scene;
class GameObject;
GameObject* CreateMissionPoint(Scene& scene, Transform transform, int missionId);