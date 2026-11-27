#pragma once

class Scene;
class GameObject;

// Player作成
// controllableがfalseの時は、カメラと操作を付けない
GameObject* CreatePlayer(Scene& scene, bool controllable = true);
void UpdatePlayer(GameObject* player, GameObject* cameraObj, float dt);