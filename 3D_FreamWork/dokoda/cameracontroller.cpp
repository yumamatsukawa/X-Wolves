#include "cameracontroller.h"
#include "../Engine/Core/GameEngine.h"
#include "../Engine/Core/Input.h"
#include"../Engine/Render/Image.h"



void CameraController::Init() {
   
}

void CameraController::Uninit() {
   
}

void CameraController::Update(float dt) {
    

    Camera& camera3D = Image::GetCamera3D();
    camera3D.position = GetOwner()->transform.position;
   
}