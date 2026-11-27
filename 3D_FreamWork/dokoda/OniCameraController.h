#pragma once
#include "../Engine/Core/Component.h"

// 鬼の位置と視線の向きに、3Dカメラを合わせる
class OniCameraController : public Component
{
private:

public:
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

};
