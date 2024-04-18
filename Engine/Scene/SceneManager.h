/**
 * @file SceneManager.h
 * @brief シーンの管理をするクラス
 * @author moomin
 * @date 2024/03/13
 */

#pragma once

#include "IScene.h"
#include "BaseScene.h"

#include "ImGuiManager.h"
#include "DX12Cmd.h"

#include <memory>

class SceneManager : public IScene
{
#pragma region メンバ変数
private:
	// シングルトンのインスタンス
	ImGuiManager* imGuiMgr_ = nullptr;// ImGuiの管理クラス
	DX12Cmd* dx12Cmd_ = nullptr;// DirectXのインスタンス

	// 現在のシーンのインスタンス
	std::unique_ptr<BaseScene> nowScene_ = nullptr;

	// 次のシーンはなにか
	Scene nextScene_ = Scene::NONE;

	// ゲームを終了させる
	bool isGameEnd_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタとデストラクタ
	SceneManager();
	~SceneManager() {}

	// 基本処理
	void Initialize();
	void Update();
	void Draw();

	// 指定のシーンに変更する
	void ChangeScene(const Scene& nextScene) override;

	// ゲームを終了させる
	void GameEnd() override;
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsGameEnd() override { return isGameEnd_; }
#pragma endregion
};