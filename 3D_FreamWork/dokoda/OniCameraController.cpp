#include "OniCameraController.h"
#include "OniController.h"
#include "../Engine/Core/GameEngine.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/MathUtil.h"
#include "../Engine/Render/Image.h"

void OniCameraController::Init() {
    Image::SetCamera3DAutoSync(false);
}

void OniCameraController::Uninit() {
    Image::SetCamera3DAutoSync(true);
}

void OniCameraController::Update(float dt) {

    GameObject* oni = GetOwner(); if (!oni) return;
    auto* controller = oni->GetComponent<OniController>(); if (!controller) return;

    // 鬼の位置から、視線の向きを見る
    Camera& camera3D = Image::GetCamera3D();
    const XMFLOAT3 pos = oni->transform.position;
    camera3D.position = pos;
    camera3D.target = pos + controller->GetLookDirection();
    camera3D.up = { 0.0f, 1.0f, 0.0f };
}
