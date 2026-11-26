#include "GameManager.h"
#include "MissionManager.h"
#include "../Engine/Core/GameObject.h"

void GameManager::Init()
{
	gameState = GameState::Ready;
	gameResult = GameResult::None;

	remainingTime = TIME_LIMIT;

	runnerCount = INITIAL_RUNNER_COUNT;
	oniCount = INITIAL_ONI_COUNT;
}

void GameManager::Update(float dt)
{
	// ゲーム進行中ではない場合は処理を行わない
	if (gameState != GameState::Playing) return;
	
	// ミッションマネージャーの保存場所
	MissionManager* missionManager = nullptr;

	if (GetOwner())
	{// OwnerからMissionMangerを取得
		missionManager = GetOwner()->GetComponent<MissionManager>();
	}

	// 現在の勝敗を確認
	CheckGameResult(missionManager);

	// 勝敗が決まった場合はここでUpdateを終了する
	if (gameState != GameState::Playing) return;

	// 残り時間を減らす
	remainingTime -= dt;

	// 残り時間が0になったときの処理
	if (remainingTime <= 0.0f)
	{
		remainingTime = 0.0f;

		// 時間切れ、勝敗の確認
		CheckTimeUpResult(missionManager);
	}
}

void GameManager::Uninit()
{

}

void GameManager::StartGame()
{
	// 準備状態以外の場合は開始しない
	if (gameState != GameState::Ready) return;

	// ゲームの状態をPlayingに変える
	gameState = GameState::Playing;

	// 他の状態を初期化させる (Resetを呼び出すため不要ではある)
	gameResult = GameResult::None;
	remainingTime = TIME_LIMIT;
	runnerCount = INITIAL_RUNNER_COUNT;
	oniCount = INITIAL_ONI_COUNT;
}

void GameManager::Reset()
{
	// 待機状態にする
	gameState = GameState::Ready;

	// 他の状態を初期化する
	gameResult = GameResult::None;
	remainingTime = TIME_LIMIT;
	runnerCount = INITIAL_RUNNER_COUNT;
	oniCount = INITIAL_ONI_COUNT;
}

void GameManager::FinishGame(GameResult result)
{
	if (gameState == GameState::Result) return;

	if (result == GameResult::None) return;

	// 勝敗を確定
	gameResult = result;
	
	// ゲームの状態をResultに変える 
	gameState = GameState::Result;
}

bool GameManager::ChangeRunnerToOni()
{
	// ゲーム中以外は人数変更は行わない
	if (gameState != GameState::Playing) return false;

	// 逃走者が残っていない場合は変更しない
	if (runnerCount <= 0) return false;

	// 逃走者が1人減り、鬼が1人増える
	runnerCount--;
	oniCount++;

	return true;
}

void GameManager::CheckGameResult(const MissionManager* missionManager)
{
	// 逃走者が0人 = 全員が見つかった
	if (runnerCount <= 0)
	{
		// 鬼の勝利でゲーム終了
		FinishGame(GameResult::OniWin);
		return;
	}

	// ミッションマネージャーがない場合 (安全のため)
	if (!missionManager) return;

	// ミッションをすべてクリアした場合
	if (missionManager->AreAllMissionCompleted())
	{
		// 逃走者の勝利でゲーム終了
		FinishGame(GameResult::RunnerWin);
	}
}

void GameManager::CheckTimeUpResult(const MissionManager* missionManager)
{
	// 逃走者が0人 = 全員が見つかった
	// TimeUpギリギリで全員見つかった場合は鬼の勝利を優先する (変更可能)
	if (runnerCount <= 0)
	{
		// 鬼の勝利でゲーム終了
		FinishGame(GameResult::OniWin);
		return;
	}

	// MissionManagerがない場合はDrawとして処理 (安全のため)
	if (!missionManager)
	{
		FinishGame(GameResult::Draw);
		return;
	}

	// Missionが7個以上達成されている場合
	if (missionManager->AreEnoughMissionCompleted())
	{
		// 逃走者の勝利でゲーム終了
		FinishGame(GameResult::RunnerWin);
		return;
	}

	// それ以外の場合は引き分け
	FinishGame(GameResult::Draw);
}
 