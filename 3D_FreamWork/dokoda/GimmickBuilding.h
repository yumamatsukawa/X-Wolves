#pragma once

#include<DirectXMath.h>

class Scene;
class GameObject;

GameObject* CreateGimmickBuilding(
	Scene& scene,
	const DirectX::XMFLOAT3& position
);
