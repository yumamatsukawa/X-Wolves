#include "TitleScene.h"
#include "StartButton.h"

void TitleScene::Init()
{
    CreateStartButton(*this);
}

void TitleScene::Uninit()
{
    Scene::Uninit();
}
