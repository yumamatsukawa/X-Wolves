#include "MissionManager.h"

void MissionManager::Init()
{
	for (int i = 0; i < MISSION_COUNT; i++)
	{
		// 0～9を仮MissionIDとして使用する
		missions[i].id = i;

		// 全Missionを未クリア状態に戻す
		missions[i].isCompleted = false;
	}
}

void MissionManager::Update(float dt)
{
	// MissionManagerはMissionの状態管理のみを担当するため、
	// 現段階では毎フレームの更新処理は行わない
}

void MissionManager::Uninit()
{

}

void MissionManager::Reset()
{
	for (int i = 0; i < MISSION_COUNT; i++)
	{
		// 0～9を仮MissionIDとして使用する
		missions[i].id = i;

		// 全Missionを未クリア状態に戻す
		missions[i].isCompleted = false;
	}
}


bool MissionManager::CompleteMission(MissionID missionID)
{
	// MissionIDから配列上の位置を取得
	int missionIndex = FindMissionIndex(missionID);

	// ミッションが存在しない場合はfalse
	if (missionIndex < 0) return false;

	// すでにクリア済みの場合もfalse
	if (missions[missionIndex].isCompleted) return false;

	// Missionをクリア状態にする
	missions[missionIndex].isCompleted = true;

	return true;
}

bool MissionManager::HasMission(MissionID missionID) const
{
	return FindMissionIndex(missionID) >= 0;
}

bool MissionManager::IsMissionCompleted(MissionID missionID) const
{
	// MissionIDから配列上の位置を取得
	int missionIndex = FindMissionIndex(missionID);

	// 存在しないMissionIDの場合はfalse
	if (missionIndex < 0) return false;

	return missions[missionIndex].isCompleted;
}

int MissionManager::GetCompletedMissionCount() const
{
	// クリアされているミッションの数
	int completedCount = 0;

	for (const MissionState& mission : missions)
	{
		// そのミッションがクリアされている場合
		if (mission.isCompleted)
		{
			completedCount++;
		}
	}

	return completedCount;
}

int MissionManager::GetRemainingMissionCount() const
{
	return MISSION_COUNT - GetCompletedMissionCount();
}


bool MissionManager::AreEnoughMissionCompleted() const
{
	return GetCompletedMissionCount() >= CLEAR_COUNT;
}

bool MissionManager::AreAllMissionCompleted() const
{
	return GetCompletedMissionCount() >= MISSION_COUNT;
}

int MissionManager::FindMissionIndex(MissionID missionID) const
{
	for (int i = 0; i < MISSION_COUNT; i++)
	{
		if (missions[i].id == missionID)
		{
			return i;
		}
	}

	// MissionIDが見つからなかった
	return -1;
}