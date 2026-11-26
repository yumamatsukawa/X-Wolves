#pragma once
#include "../Engine/Core/Component.h"
#include <string>

enum class MissionType { Emote, Click, Item };

class MissionComponent : public Component
{
private:
	int missionID = 0;

	MissionType type = MissionType::Click;
	std::wstring emoteTag;     // エモートのタグ
	int keyID = 0;             // クリックのキー
	unsigned int itemID = 0;   // 必要アイテム
	float duration = 0.0f;
	int count = 1;
	float radius = 1.0f;       // ミッションポイントの反応半径(座標はCSVからownerのtransformへ反映)

	// 進捗
	float elapsed = 0.0f;
	int done = 0;
	bool counted = false;   // 今の連続入力を回数に数え済みか
	bool cleared = false;

	void CheckClear();
	void UpdateEmote(float dt);
	void UpdateClick(float dt);
	void UpdateItem(float dt);

public:
	MissionComponent(int id) {
		SetMissionID(id);
	}

	void SetMissionID(int id) { missionID = id; }   // AddComponentの直後(Initより前)に呼ぶ
	void Init() override;
	void Update(float dt) override;
};