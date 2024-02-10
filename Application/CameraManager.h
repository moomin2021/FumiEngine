#pragma once
/**
 * @file Player.h
 * @brief カメラを管理するクラス
 * @author moomin
 * @date 2024/02/10
 */

#include "Camera.h"

#include <memory>

class CameraManager
{
#pragma region メンバ変数
private:
	// 現在のカメラ
	Camera* nowCamera_ = nullptr;

	// 遷移時のカメラ
	std::unique_ptr<Camera> tranCamera_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(Camera* inCamera);

	// 行列更新処理
	void MatUpdate();

	// カメラ変更
	void ChangeCamera(Camera* inCamera);

	CameraManager() {}
	~CameraManager() {}
#pragma endregion
};