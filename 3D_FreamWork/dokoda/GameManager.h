#pragma once

#include "../Engine/Core/Component.h"

class GameManager : public Component 
{
public:
	// ゲームの状態
	enum class GameState
	{
		Ready,		// ゲーム開始前
		Playing,	// ゲーム中
		Result		// リザルト
	};

	// ゲーム結果
	enum class GameResult
	{
		None,		// 勝敗が決まっていない
		OniWin,		// 鬼勝利
		RunnerWin,	// 逃走者勝利
		Draw		// 引き分け
	};

private:
	GameState gameState = GameState::Ready;		// 現在のゲームの状態
	GameResult gameResult = GameResult::None;	// 現在のゲーム結果
	static constexpr float TIME_LIMIT = 180.0f;	// 制限時間 (3分)
	float remainingTime = TIME_LIMIT;			// 実際の残り時間

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;

	void StartGame();
	void FinishGame(GameResult result);

	// 他のオブジェクトから参照する可能性がある関数
	GameState GetState() const { return gameState; }			// 現在のゲームの状態を取得
	GameResult GetResult() const { return gameResult;}			// 現在のゲーム結果を取得
	float GetRemainingTime() const { return remainingTime; }	// 現在の残り時間を取得
};

