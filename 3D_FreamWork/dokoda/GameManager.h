#pragma once

#include "../Engine/Core/Component.h"

class MissionManager;

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
	// ゲーム時間
	GameState gameState = GameState::Ready;		// 現在のゲームの状態
	GameResult gameResult = GameResult::None;	// 現在のゲーム結果
	static constexpr float TIME_LIMIT = 300.0f;	// 制限時間 (5分)
	float remainingTime = TIME_LIMIT;			// 実際の残り時間

	// プレイヤー人数
	static constexpr int INITIAL_RUNNER_COUNT = 3;	// 初期逃走者人数
	static constexpr int INITIAL_ONI_COUNT = 1;		// 初期鬼人数
	int runnerCount = INITIAL_RUNNER_COUNT;	// 現在の逃走者人数
	int oniCount = INITIAL_ONI_COUNT;		// 現在の鬼人数

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;

	// マッチ開始
	void StartGame();

	// 状態のリセット
	void Reset();

	// ゲーム終了
	void FinishGame(GameResult result);

	// 逃走者が捕まり、鬼になったときの人数変更
	bool ChangeRunnerToOni();

	// 他のオブジェクトから参照する可能性がある関数
	GameState GetState() const { return gameState; }			// 現在のゲームの状態を取得
	GameResult GetResult() const { return gameResult;}			// 現在のゲーム結果を取得
	float GetRemainingTime() const { return remainingTime; }	// 現在の残り時間を取得
	int GetRunnerCount() const { return runnerCount; }			// 現在の逃走者数を取得
	int GetOniCount() const { return oniCount; }				// 現在の鬼の人数を取得

private:
	void CheckGameResult(const MissionManager* missionManager);		// 現在のゲームの勝敗を確認
	void CheckTimeUpResult(const MissionManager* missionManager);	// 時間切れのときの勝敗を確認
};

