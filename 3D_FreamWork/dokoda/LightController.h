#pragma once
#include"../Engine/Core/Component.h"
#include"../Engine/Render3D/LightComponent.h"
#include <DirectXMath.h>

class LightController : public Component 
{
private:
	LightComponent* light = nullptr;

	//ˆê•bŠÔ‚Ì‰ñ“]Šp“x
	float rotateSpeed = 90.0f;

	//ã‰º•ûŒü‚Ì§ŒÀ
	float minPitch = -80.0f;
	float maxPitch = 80.0f;


	//‹S‚ğÆ‚ç‚µ‚Ä‚¢‚é‚©
	bool hitOni = false;
	//–Ú‚­‚ç‚Ü‚µó‘Ô
	bool oniBlind = false;
	//–Ú‚­‚ç‚Ü‚µŠÔ
	float blindTimer = 0.0f;
	//–Ú‚­‚ç‚Ü‚µŒo‰ßŠÔ
	float blindDuration = 3.0f;



public:
	void Init() override;
	void Update(float dt) override;

	//‹S‚ÉŒõ‚ª“–‚½‚Á‚½
	void HitOni();

	bool IsoniBlind() const
	{
		return oniBlind;
	}

};

