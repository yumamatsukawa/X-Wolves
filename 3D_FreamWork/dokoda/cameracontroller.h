#pragma once
#include "../Engine/Core/Component.h"
#include <DirectXMath.h>


class CameraController : public Component 
{
private:
  

public:
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

};
