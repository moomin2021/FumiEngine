#pragma once
#include "Object3D.h"

#include <memory>

class Dissolve3D {
#pragma region 構造体
private:
	struct ConstBufferData {
		float dissolveTime;
	};
#pragma endregion

#pragma region メンバ変数
private:
	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	// 定数バッファ
	ComPtr<ID3D12Resource>	constBuff_ = nullptr;	// 定数バッファ
	ConstBufferData* constMap_;	// マッピング処理用

	// ディゾルブ係数
	float time_ = 0.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
#pragma endregion

#pragma region セッター関数
public:
	// ディゾルブ係数を設定
	void SetDissolveTime(float t) { time_ = t; }
#pragma endregion
};