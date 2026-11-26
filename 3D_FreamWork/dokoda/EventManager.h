#pragma once

#include "../Engine/Core/Component.h"

class GameManager;

class EventManager : public Component
{
public:
	// イベントの種類
	enum class EventType
	{
		None,				// イベントなし
		NpcMoveChange,		// NPCの行動変化
		Jukebox,			// ジュークボックス操作
		Bomb,				// 爆弾解体
		Fireworks,			// 花火
		Blackout			// 停電
	};

private:
	// イベントが発生する残り時間 (2分)
	static constexpr float EVENT_TRIGGER_TIME = 120.0f;

	// 現在のイベント
	EventType currentEvent = EventType::None;

	// このマッチですでにイベント判定を行ったか
	bool eventDecided = false;

	// GameManager
	GameManager* gameManager = nullptr;

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;

	void Reset();

	EventType GetCurrentEvent() const{	return currentEvent; }				// 現在選択されているイベントを取得
	bool IsEventDecided() const { return eventDecided; }					// このマッチでイベントを行ったかを取得
	
private:
	EventType DecideEvent(int runnerCount, int oniCount) const;	// 現在の人数からイベントを決定
	void PublishEvent(EventType eventType);					// 決定したイベントをEventBusに送る
};
