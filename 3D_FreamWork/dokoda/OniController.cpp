#include"OniController.h"
#include"../Engine/Core/GameObject.h"
#include"../Engine/Core/Scene.h"
#include"cameracontroller.h"


GameObject* CreateCamera(Scene& scene,CameraMode mode)
{
	auto* OniCamera = scene.CreateObject();
	auto* controller = OniCamera->AddComponent<CameraController>();

	switch (mode)
	{
	case CameraMode::TPS:
		OniCamera->transform.position = { 0.0f,3.0f,-8.0f };
		break;
	case CameraMode::Overhead:
		
	default:
		OniCamera->transform.position = { 0.0f,10.0f,0.0f };
		break;
	}



	/*OniCamera->transform.position = { 0.0f, 10.0f, 0.0f };

	OniCamera->AddComponent<CameraController>();*/

	return OniCamera;
}


GameObject* CreateOni(Scene& scene)
{
	auto* oni = scene.CreateObject();
	oni->transform.position = { 0.0f,0.0f,0.0f };
	return oni;
}