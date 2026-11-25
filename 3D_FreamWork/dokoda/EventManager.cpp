#include "EventManager.h"

#include "GameManager.h"

#include "../Engine/Core/GameObject.h"
#include "../Engine/Core/EventBus.h"
#include "../Engine/Core/MathUtil.h"
#include "../Engine/Core/Time.h"

namespace
{
	// EventBusで使用するイベント名 (仮)
	// Event側に合わせて変更
	constexpr const char* EVENT_01_NAME = "EVENT-01";
	constexpr const char* EVENT_02_NAME = "EVENT-02";
	constexpr const char* EVENT_03_NAME = "EVENT-03";
	constexpr const char* EVENT_04_NAME = "EVENT-04";
	constexpr const char* EVENT_05_NAME = "EVENT-05";
}

void EventManager::Init()
{
	// 同じGameObjectに存在するGameManagerを取得
	if (GetOwner())
	{
		gameManager = GetOwner()->GetComponent<GameManager>();
	}

	currentEvent = EventType::None;
	eventDecided = false;
}

void EventManager::Update(float dt)
{
	// GameManagerがない場合は処理しない
	if (!gameManager) return;

	// ゲーム中以外は処理しない
	if (gameManager->GetState() != GameManager::GameState::Playing) return;

	// ポーズ中は処理しない
	if (Time::IsPaused()) return;

	// すでにイベントを行っっている場合は処理しない
	if (eventDecided) return;

	// 残り時間が2分以上ある場合は処理しない
	if (gameManager->GetRemainingTime() > EVENT_TRIGGER_TIME) return;

	// 時間切れの場合は処理しない
	if (gameManager->GetRemainingTime() <= 0.0f) return;

	// ↓イベントを行う

	// 現在の人数からイベントを決定
	currentEvent = DecideEvent(
		gameManager->GetRunnerCount(),
		gameManager->GetOniCount()
	);

	// フラグをtrueに変更
	eventDecided = true;

	// イベントがない場合は終了
	if (currentEvent == EventType::None) return;

	// EventBusに行うイベントを送る
	PublishEvent(currentEvent);
}

void EventManager::Uninit()
{
	// GameManagerへの参照を解除
	gameManager = nullptr;
}

void EventManager::Reset()
{
	// イベント未発生状態に戻す
	currentEvent = EventType::None;
	eventDecided = false;
}

EventManager::EventType EventManager::DecideEvent(int runnerCount, int oniCount) const
{
	// 逃走者3, 鬼1
	if (runnerCount == 3 && oniCount == 1)
	{// NPCの動きを変える
		return EventType::NpcMoveChange;
	}

	// 逃走者2, 鬼2
	if (runnerCount == 2 && oniCount == 2)
	{// ジュークボックスか爆弾
		int randomValue = Math::RandomRange(0, 1);

		if (randomValue == 0) return EventType::Jukebox;

		return EventType::Bomb;
	}

	// 逃走者1, 鬼3
	if (runnerCount == 1 && oniCount == 3)
	{// 花火か停電
		int randomValue = Math::RandomRange(0, 1);

		if (randomValue == 0) return EventType::Fireworks;

		return EventType::Blackout;
	}

	// それ以外の場合
	return EventType::None;
}


void EventManager::PublishEvent(EventType eventType)
{
	switch (eventType)
	{
	case EventType::NpcMoveChange:
		EventBus::Get().Publish(EVENT_01_NAME);
		break;

	case EventType::Jukebox:
		EventBus::Get().Publish(EVENT_02_NAME);
		break;

	case EventType::Bomb:
		EventBus::Get().Publish(EVENT_03_NAME);
		break;

	case EventType::Fireworks:
		EventBus::Get().Publish(EVENT_04_NAME);
		break;

	case EventType::Blackout:
		EventBus::Get().Publish(EVENT_05_NAME);
		break;

	case EventType::None:
	default:
		break;
	}
}
