#pragma once
#include <DirectXMath.h>

class Scene;
class GameObject;


// 地面を作る(動かない四角。タグ"Ground"なので、見張り役(VisionComponent)の視線をさえぎる)
GameObject* CreateGround(Scene& scene, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);