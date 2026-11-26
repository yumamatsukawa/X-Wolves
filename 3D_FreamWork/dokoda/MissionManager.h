#pragma once

#include "../Engine/Core/Component.h"

#include <array>

class MissionManager : public Component
{
public:
	using MissionID = int;

	struct MissionState
	{
		MissionID id;
		bool isCompleted;
	};

private:
	// ミッション数
	static constexpr int MISSION_COUNT = 10;	
	static constexpr int CLEAR_COUNT = 7;

	// 現在選択されているミッション
	std::array<MissionState, MISSION_COUNT> missions{};

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;
	
	void Reset();

	bool CompleteMission(MissionID missionID);		// ミッションをクリア状態にする (できない場合はfalse)
	bool HasMission(MissionID missionID) const;		// ミッションが存在するかチェック
	bool IsMissionCompleted(MissionID missionID) const;	// ミッションがクリア済みかチェック
	int GetCompletedMissionCount() const;			// クリア済みのミッション総数を取得	
	int GetRemainingMissionCount() const;			// 残っているミッション数を取得

	int GetMissionCount() const { return MISSION_COUNT; }	// ミッション総数

	bool AreEnoughMissionCompleted() const;			// 7個以上のミッションがクリアされているか
	bool AreAllMissionCompleted() const;			// 全てのミッションがクリアされているか

private:
	int FindMissionIndex(MissionID missionID) const;	// IDから配列の位置を取得
};