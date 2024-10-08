#include "SceneManager.h"

#include "GameScene.h"
#include "Key.h"

SceneManager::SceneManager()
{
	// ImGuiの管理クラスのインスタンスを取得
	imGuiMgr_ = ImGuiManager::GetInstance();

	// DirectXのインスタンスを取得
	dx12Cmd_ = DX12Cmd::GetInstance();
}

void SceneManager::Initialize()
{
	// 最初のシーンの生成と初期化
	nowScene_ = std::make_unique<GameScene>(this);
	nowScene_->Initialize();
}

void SceneManager::Update()
{
	// 次のシーンが設定されたら
	if (nextScene_ != Scene::NONE)
	{
		// 現在のシーンの終了処理を実行
		nowScene_->Finalize();
		nowScene_.reset();

		// シーンの生成
		switch (nextScene_)
		{
			case Scene::GAME:
				nowScene_ = std::make_unique<GameScene>(this);
				break;
		}

		// シーンの初期化
		nowScene_->Initialize();

		// 次のシーンをクリア
		nextScene_ = Scene::NONE;
	}

	if (Key::GetInstance()->TriggerKey(DIK_2)) GameEnd();

	// ImGuiの処理開始
	imGuiMgr_->Begin();

	// 現在のシーンを更新
	nowScene_->Update();

	// ImGuiの処理
	nowScene_->ImGuiUpdate();
}

void SceneManager::Draw()
{
	// 描画前処理
	dx12Cmd_->PreDraw();

	// 現在のシーンの描画
	nowScene_->Draw();

	// ImGuiの終了処理と描画処理
	imGuiMgr_->End();
	imGuiMgr_->Draw();

	// 描画後後処理
	dx12Cmd_->PostDraw();
}

void SceneManager::ChangeScene(const Scene& nextScene)
{
	// 次のシーンを設定
	nextScene_ = nextScene;
}

void SceneManager::GameEnd()
{
	isGameEnd_ = true;
}