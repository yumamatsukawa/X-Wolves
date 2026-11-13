#include "Map.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Render3D/MeshRenderer.h"
#include "../Engine/Physics/RigidbodyComponent.h"

GameObject* CreateGround(Scene& scene) {
    // タグを"Ground"にしておくと、VisionComponentの視線がさえぎられる(付け忘れると素通しになる)
    GameObject* Ground = scene.CreateObject("Ground", "Ground");
    Ground->transform.position = { 0.0f, 0.0f, 0.0f };
    Ground->transform.scale = { 100.0f, 1.0f, 100.0f };

    // 見た目(画像を引き伸ばして、灰色にする)
    auto* sprite = Ground->AddComponent<MeshRenderer>(Mesh::CreateCube());
   
	Ground->AddComponent<BoxRigidbodyComponent>(BodyType::Static);   // 動かない物体にする
    return Ground;
}