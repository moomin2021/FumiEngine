#include "SceneManager.h"
#include "Scene1.h"
#include "DX12Cmd.h"
#include "PipelineManager.h"

// インスタンス取得
SceneManager* SceneManager::GetInstance()
{
	// 関数内static変数として宣言
	static SceneManager instance;
	return &instance;
}

// コンストラクタ
SceneManager::SceneManager() :
#pragma region 初期化リスト
	// キーボード入力クラス
	key_(nullptr),

	// 現在のシーン
	nowScene_(nullptr),

	// ポストエフェクト
	gaussianPostEffect_(nullptr),
	highLumiPostEffect_(nullptr),
	bloomPostEffect_(nullptr),

	// ポストエフェクトの種類
	postEffectType_(PostEffectType::NORMAL)
#pragma endregion
{
	// インスタンス取得
	key_ = Key::GetInstance();

	// 最初のシーン
	nowScene_ = std::make_unique<Scene1>();
	nowScene_->Initialize();

	gaussianPostEffect_ = std::make_unique<PostEffect>();
	highLumiPostEffect_ = std::make_unique<PostEffect>();
	bloomPostEffect_ = std::make_unique<PostEffect>();

	PipelineManager::GetInstance();
}

// デストラクタ
SceneManager::~SceneManager() {
	
}

void SceneManager::ChangeScene(int changeSceneNum)
{
	switch (changeSceneNum)
	{
	case SCENE::SCENE1:
		nowScene_ = std::make_unique<Scene1>();
		nowScene_->Initialize();
		break;
	}
}

// 更新処理
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) postEffectType_ = PostEffectType::NORMAL;
	if (key_->TriggerKey(DIK_2)) postEffectType_ = PostEffectType::BLUR;
	if (key_->TriggerKey(DIK_3)) postEffectType_ = PostEffectType::BLOOM;

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	if (PostEffectType::NORMAL == postEffectType_) {
		// --描画前処理-- //
		DX12Cmd::GetInstance()->PreDraw();

		nowScene_->Draw();

		// --描画後処理-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	else if (PostEffectType::BLUR == postEffectType_) {
		gaussianPostEffect_->PreDraw();

		nowScene_->Draw();

		gaussianPostEffect_->PostDraw();

		// --描画前処理-- //
		DX12Cmd::GetInstance()->PreDraw();

		PipelineManager::GetInstance()->PreDraw("Gaussian");

		gaussianPostEffect_->Draw();

		// --描画後処理-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	else if (PostEffectType::BLOOM == postEffectType_) {
		highLumiPostEffect_->PreDraw();

		nowScene_->Draw();

		highLumiPostEffect_->PostDraw();

		bloomPostEffect_->PreDraw();

		PipelineManager::GetInstance()->PreDraw("HighLumi");
		highLumiPostEffect_->Draw();

		bloomPostEffect_->PostDraw();

		// --描画前処理-- //
		DX12Cmd::GetInstance()->PreDraw();

		PipelineManager::GetInstance()->PreDraw("Bloom");

		bloomPostEffect_->Draw();

		// --描画後処理-- //
		DX12Cmd::GetInstance()->PostDraw();
	}
}
