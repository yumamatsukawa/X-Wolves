
#include "Oni.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Physics/RigidbodyComponent.h"
#include "../Engine/Render/Image.h"
#include "OniController.h"
#include "OniCameraController.h"

GameObject* CreateOni(Scene& scene)
{
	Image::SetCamera3DAutoSync(false);

	GameObject* oni = scene.CreateObject("Oni", "Oni");
	oni->transform.position = { 0.0f, 15.0f, -15.0f };

	auto* rigidbody = oni->AddComponent<SphereRigidbodyComponent>(
		BodyType::Dynamic,
		oni->transform.scale.x / 2,
		100.0f
	);
	rigidbody->SetUseGravity(false);
	rigidbody->SetFreezeRotation(true);

	oni->AddComponent<OniController>();
	oni->AddComponent<OniCameraController>();

	return oni;
}
