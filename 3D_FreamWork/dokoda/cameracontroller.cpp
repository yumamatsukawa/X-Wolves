#include "cameracontroller.h"
#include "../Engine/Core/GameEngine.h"
#include "../Engine/Core/Input.h"
#include"../Engine/Render/Image.h"



void CameraController::Init() {
    Image::SetCamera3DAutoSync(false);
}

void CameraController::Uninit() {
    Image::SetCamera3DAutoSync(true);
}

void CameraController::Update(float dt) {

    Camera& camera3D = Image::GetCamera3D();

    XMFLOAT3 pos = GetOwner()->transform.position;
    camera3D.position = { pos.x, pos.y + 100 , pos.z - 100};
    camera3D.target = { pos.x, pos.y , pos.z + 1.0f };
    camera3D.up = { 0, 0, 1};
}