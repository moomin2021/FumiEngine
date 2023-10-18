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

	// ルート探索
	//std::vector<Vector3> RouteSearch(int32_t startID, int32_t endID);
	void RouteSearch(int32_t startID, int32_t endID, std::vector<Vector3>& outputRoute);

private:
	// セルを作成
	void CreateCell();

	// セルをリンク
	void LinkCell();

	// 指定したIDのセルのポインタを取得
	NavCell* GetNavCell(int32_t cellID);

	// 指定したセル二つの直線距離を返す
	float CalcCellDist(int32_t startID, int32_t endID);
#pragma endregion
};

enum class NodeState {
	OPEN,
	CLOSE,
};

struct NavNode {
	// 前のノード
	NavNode* parent = nullptr;

	// ノードの状態
	NodeState state = NodeState::OPEN;

	// セル
	NavCell* cell = nullptr;

	float cCost = 0.0f;// 移動コスト
	float hCost = 0.0f;// 推定コスト
	float score = 0.0f;// スコア = 移動コスト + 推定コスト

	// スコアを取得
	float GetScore() { return score = cCost + hCost; }
};