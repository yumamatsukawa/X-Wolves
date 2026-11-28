#pragma once
#include "../Engine/Core/Scene.h"
#include "../Engine/Render3D/Light.h"

// タイトル画面のシーン
class GameScene : public Scene
{
private:
    Light originalLight;   // 暗くする前の平行光源

public:
    void Init()           override;
    void Uninit()         override;
};
