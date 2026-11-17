#pragma once
#include "../Engine/Core/Component.h"

struct MissionConditions
{
	int keyID;	  // ミッションのキー入力
	unsigned int itemID;  // 必要アイテム
	std::string emoteTag; // エモートのタグ
};

class MissionComponent : public Component
{
private:
	int missionID;
	MissionConditions missionConditions{};

public:
	MissionComponent() {}
	~MissionComponent() {}

	void Init() override;
	void Update(float dt) override;
};