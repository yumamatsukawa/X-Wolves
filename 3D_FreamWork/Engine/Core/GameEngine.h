// GameEngine.h
#pragma once
#include <windows.h>
#include "Scene.h"

class GameEngine {
private:
    GameEngine() = default;
    Scene    scene;

    bool isRunning = false;
public:
    static GameEngine& Get() {          // シングルトン
        static GameEngine instance;
        return instance;
    }

    bool Init(HWND hwnd, int w, int h);
    void Run();   // メインループ
    void Uninit();

    Scene& GetScene() { return scene; }

    // 現在のシーンから検索する(詳細はScene.hの検索の項)
    GameObject* FindObject(const std::string& name) const { return scene.FindObject(name); }
    GameObject* FindObjectWithTag(const std::string& tag) const { return scene.FindObjectWithTag(tag); }
    std::vector<GameObject*> FindObjects(const std::string& name) const { return scene.FindObjects(name); }
    std::vector<GameObject*> FindObjectsWithTag(const std::string& tag) const { return scene.FindObjectsWithTag(tag); }
};