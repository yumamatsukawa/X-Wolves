#pragma once

class Scene;
class GameObject;

GameObject* CreateManagerObject(Scene& scene);

// マッチ開始
bool StartMatch(GameObject* managerObject);