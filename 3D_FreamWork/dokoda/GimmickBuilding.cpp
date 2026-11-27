#include"../Engine/Core/Scene.h"
#include"../Engine/Core/GameObject.h"
#include"../Engine/Render/ParticleRenderer.h"

#include"GimmickBuilding.h"
#include"GimmickController.h"

using namespace DirectX;

GameObject* CreateGimmickBuilding(
	Scene& scene,
	const DirectX::XMFLOAT3& position
)
{
	//GimmickObject
	auto* GimmickObject = scene.CreateObject();
	
	GimmickObject->transform.position = position;
	GimmickObject->SetTag("Gimmick");

	//Particle
	ParticleSettings settings;

	settings.texture = L"Assets/particle.png";

	settings.worldSpace = true;
	settings.use3D = true;

	//Particleの発射方向
	//Y+方向なので上方向へ飛ぶ
	settings.direction = { 0.0f,1.0f,0.0f };

	//directionを中心に+-15、合計３０の範囲内
	settings.spreadAngle = 30.0f;

	//粒の速さ(1秒に動く距離。この間でランダム)
	settings.speedMin = 50.0f;
	settings.speedMax = 100.0f;

	// 粒が消えるまでの時間(秒。この間でランダム)
	settings.lifetimeMin = 0.5f;
	settings.lifetimeMax = 1.0f;

	// 出た時と消える時の大きさ(寿命に沿って変わる。0以下になった粒は描かない)
	settings.startSize = 1.0f;
	settings.endSize = 0.0f;

	//発生した直後は不透明な白
	settings.startColor = {1.0f, 1.0f, 1.0f, 1.0f };

	// 寿命終了時は透明な白
	// RGBは同じでAlphaだけ0になるので、
	// 徐々にフェードアウトする
	settings.endColor = { 1.0f, 1.0f, 1.0f, 0.0f };

	// 1秒に出す数(Playしている間だけ出る。Burstには関係ない)
	settings.emitRate = 10.0f;  

	// 重力(1秒に速度が変わる量。粒が落ちていく)
	settings.gravity = { 0.0f, 0.0f, 0.0f };     
	
	//ParticleRendererにParticleSettingsを渡して追加する
	GimmickObject->AddComponent<ParticleRenderer>(settings);

	// GimmickController::Init()内で
    // GetComponent<ParticleRenderer>()
    // を実行した時、ParticleRendererを取得できる
	GimmickObject->AddComponent<GimmickController>();

	return GimmickObject;
}