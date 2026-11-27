#pragma once
class Scene;
class GameObject;

class Scene;
class GameObject;

GameObject* CreateOni(Scene& scene);
void UpdateOni(GameObject* oni, GameObject* cameraObj, float dt);

bool CanOniSeePlayer(const GameObject* oni, const GameObject* player);