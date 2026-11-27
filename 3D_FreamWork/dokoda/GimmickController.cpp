#include"GimmickController.h"



void GimmickController::Init()
{
	particle = GetOwner()->GetComponent<ParticleRenderer>();

	active = false;
	timer = 0.0f;

	if (particle)
	{
		particle->Stop();
	}
}

void GimmickController::Update(float dt)
{
	if (!particle) return;
	
	timer += dt;
	
	if (active)
	{
		if (timer >= playTime)
		{
			Stop();
		}
	}
	else
	{
		if (timer >= stopTime)
		{
			Play();
		}
	}
}

void GimmickController::Play()
{
	if (!particle) return;

	active = true;
	timer = 0.0f;
	particle->Play();
}

void GimmickController::Stop()
{
	if (!particle) return;

	active = false;
	timer = 0.0f;
	particle->Stop();

}

void GimmickController::Burst(int count)
{
	if (!particle) return;

	particle->Burst(count);
}