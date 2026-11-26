#pragma onc
#include"../Engine/Core/GameObject.h"
#include"../Engine/Render/ParticleRenderer.h"


class GimmickController : public Component
{
private:
	ParticleRenderer* particle = nullptr;

	bool active = false;


public:

	void Init() override;
	void Update(float dt) override;

	void Play();
	void Stop();
	void Burst(int count);

	bool IsActive()
		const {
		return active;
	};
};