#include "GameManager.h"

void GameManager::Init()
{
	gameState = GameState::Ready;
	gameResult = GameResult::None;
	remainingTime = TIME_LIMIT;
}

void GameManager::Update(float dt)
{
	// ゲーム進行中の処理
	if (gameState == GameState::Playing)
	{
		// 残り時間を減らす
		remainingTime -= dt;

		// 残り時間が0になったときの処理
		if (remainingTime <= 0.0f)
		{
			// 現時点では時間切れ時の勝敗条件が未確定のため、
			// 残り時間を0にするだけでゲーム終了処理は行わない。
			// 今後MissionManagerなどからゲーム状況を取得して勝敗を判定し、
			// FinishGame()を呼び出す処理を追加する。

			remainingTime = 0.0f;
		}
	}
}

void GameManager::Uninit()
{

}

void GameManager::StartGame()
{
	// ゲームの状態をPlayingに変える
	gameState = GameState::Playing;

	// 他の状態を初期化させる
	gameResult = GameResult::None;
	remainingTime = TIME_LIMIT;
}

void GameManager::FinishGame(GameResult result)
{
	// 現時点ではNone(勝敗未確定)の場合でも終了可能

	// 勝敗を確定
	gameResult = result;
	
	// ゲームの状態をResultに変える 
	gameState = GameState::Result;
}