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
SceneManager::SceneManager() {
	// インスタンス取得
	key_ = Key::GetInstance();

	// 最初のシーン
	nowScene_ = std::make_unique<Scene1>();
	nowScene_->Initialize();

	highLumiPostEffect_ = std::make_unique<PostEffect>();
	bloomPostEffect_ = std::make_unique<PostEffect>();

	//nowPostEffect_ = std::move(postEffect_);

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

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
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
