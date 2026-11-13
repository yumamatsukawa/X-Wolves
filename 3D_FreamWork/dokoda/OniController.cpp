#include"OniController.h"
#include"../Engine/Core/GameObject.h"
#include"../Engine/Core/Scene.h"
#include"cameracontroller.h"


GameObject* CreateCamera(Scene& scene)
{
	auto* OniCamera = scene.CreateObject();
	OniCamera->AddComponent<CameraController>();
}