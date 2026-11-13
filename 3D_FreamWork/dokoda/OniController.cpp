#include"OniController.h"
#include"../Engine/Core/GameObject.h"
#include"../Engine/Core/Scene.h"
#include"cameracontroller.h"


GameObject* CreateCamera(Scene& scene)
{
	auto* OniCamera = scene.CreateObject();
	OniCamera->transform.position = { 0.0f, 10.0f, 0.0f };

	OniCamera->AddComponent<CameraController>();

	return OniCamera;
}