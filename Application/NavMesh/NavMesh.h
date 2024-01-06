#pragma once
#include "Model.h"
#include "Object3D.h"

#include "NavCell.h"
#include "Line3D.h"

#include <memory>
#include <algorithm>

class NavMesh {
#pragma region メンバ変数
private:
	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト3D
	std::unique_ptr<Object3D> object_ = nullptr;

	// セル
	std::vector<std::unique_ptr<NavCell>> cells_ = {};

	std::vector<Vector3> cellsCenter_ = {};

	// リンクライン
	std::unique_ptr<Line3D> linkLines_ = nullptr;

	// リンクライン描画フラグ
	bool isLinkLineDraw_ = false;
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
	bool RouteSearch(const Vector3& startVec, const Vector3& goalVec, std::vector<Vector3>& outputRoute);

	// レイがどのセルの上にいるのかを返す
	int32_t CheckRay2Cell(const Ray& ray);

	void AddVertex(std::string fileName, const Vector3& offset, float rota);

	// セルをリンク
	void LinkCell();
private:
	// セルを作成
	void CreateCell();

	// 指定したIDのセルのポインタを取得
	NavCell* GetNavCell(int32_t cellID);

	// 指定したセル二つの直線距離を返す
	float CalcCellDist(int32_t startID, int32_t endID);
#pragma endregion

#pragma region セッター関数
public:
	void SetIsDraw(bool frag) { object_->SetIsDraw(frag); }
	void SetIsLinkLineDraw(bool frag) { isLinkLineDraw_ = frag; }
#pragma endregion

#pragma region ゲッター関数
	const std::vector<Vector3>& GetCellsCenter() { return cellsCenter_; }
#pragma endregion
};

enum class NodeState {
	OPEN,
	CLOSE,
};

struct NavNode {
	// 前のノード
	std::shared_ptr<NavNode> parent = nullptr;

	// セルID
	int32_t cellID = ID_NONE;

	// セル
	std::shared_ptr<NavCell> cell = nullptr;

	float cCost = 0.0f;// 移動コスト
	float hCost = 0.0f;// 推定コスト
	float score = 0.0f;// スコア = 移動コスト + 推定コスト

	NavNode() : cellID(0), cCost(0.0f), hCost(0.0f) {}
	NavNode(int32_t inCellID) : cellID(inCellID), parent(nullptr), cCost(0.0f), hCost(0.0f) {}
	~NavNode() {}

	// スコアを取得
	void CalcScore() { score = cCost + hCost; }
	float GetTotalCost() { return cCost + hCost; }
};