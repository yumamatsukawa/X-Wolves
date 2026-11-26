#pragma once

class Scene;
class GameObject;
class NpcController;
class NavGridComponent;

// 敵の出現役を作る(出現した敵は、navigationの格子で壁を避けながらtargetを追いかける)
NpcController* CreateNpcSpawner(Scene& scene,NavGridComponent* navigation);
