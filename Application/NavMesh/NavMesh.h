#pragma once
#include "Model.h"
#include "Object3D.h"

#include "NavCell.h"

#include <memory>

class NavMesh {
#pragma region メンバ変数
private:
	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト3D
	std::unique_ptr<Object3D> object_ = nullptr;

	// セル
	std::vector<std::unique_ptr<NavCell>> cells_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	NavMesh() {}
	~NavMesh() {}

	// 初期化処理
	void Initialize(const std::string& fileName);

	// 行列更新処理
	void MatUpdate();

	// 描画処理
	void Draw();

private:
	// セルを作成
	void CreateCell();

	// セルをリンク
	void LinkCell();
#pragma endregion
};