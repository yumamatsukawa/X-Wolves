#include "MissionComponent.h"
#include "../Engine/Data/Csv.h"
#include "../Engine/Physics/Physics.h"
#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/Input.h"
#include <windows.h>

void MissionComponent::Init()
{
	Csv table = Csv::Load(L"Assets/Data/Missions.csv");
	int row = table.FindRow("Id", std::to_wstring(missionID));
	if (row < 0) {
		OutputDebugStringA("★ Mission: CSVに該当するIdがありません\n");
		cleared = true;   // 条件が不明なので何も反応しない
		return;
	}

	type = static_cast<MissionType>(table.GetInt(row, "Type", 1));
	emoteTag = table.GetString(row, "EmoteTag");
	keyID = table.GetInt(row, "KeyID", 0);
	itemID = static_cast<unsigned int>(table.GetInt(row, "ItemID", 0));
	duration = table.GetFloat(row, "Duration", 0.0f);
	count = table.GetInt(row, "Count", 1);
}

void MissionComponent::Update(float dt)
{
	bool nearby = Physics::OverlapSphere(GetOwner()->transform.position, GetOwner()->transform.scale.x, "Player").empty();
	if (cleared || nearby) return;

	switch (type) {
	case MissionType::Emote: UpdateEmote(dt); break;
	case MissionType::Click: UpdateClick(dt); break;
	case MissionType::Item:  UpdateItem(dt);  break;
	}

	CheckClear();
	if (cleared) {
		// ミッションCLEARのイベントバス発行
	}
}

void MissionComponent::CheckClear()
{
	if (done >= count) cleared = true;
}

void MissionComponent::UpdateEmote(float dt)
{
	// プレイヤーが emoteTag のエモート中なら elapsed += dt
	// エモート終了イベントを受けたら done++
}

void MissionComponent::UpdateClick(float dt)
{
	if (Input::GetKeyPress(keyID)) {
		elapsed += dt;
		// 押しっぱなしで連続加算されないよう、離すまで1回までにする
		if (!counted && elapsed >= duration) { done++; counted = true; }
	}
	else {
		elapsed = 0.0f;
		counted = false;
	}
}

void MissionComponent::UpdateItem(float dt)
{
	// 所持アイテムが itemID と一致していて、クリックされたら done++
}