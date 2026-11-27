#pragma once

class Scene;
class GameObject;

// PlayerçÏê¨
GameObject* CreatePlayer(Scene& scene);
void UpdatePlayer(GameObject* player, GameObject* cameraObj, float dt);