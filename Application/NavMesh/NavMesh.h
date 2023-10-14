#pragma once
#include "Model.h"
#include "Vector3.h"

#include "NavCell.h"

#include <vector>
#include <memory>

class NavMesh
{
#pragma region メンバ変数
private:
	// 頂点データ
	std::vector<Vector3> vertices_ = {};

	// 頂点インデックス
	std::vector<Vector3> indexes_ = {};

	// セル
	std::vector<std::shared_ptr<NavCell>> cells_;
#pragma endregion

#pragma region メンバ関数
public:
	// メッシュを読み込む
	void LoadMesh(std::string name);

	// セルを作成
	void CreateCell();

	// セルをリンク
	void LinkCell();

	void CreateNode(int32_t startCellID, int32_t endCellID);
#pragma endregion
};

enum NodeState {
	None,
	Open,
	Closed,
};

struct NavNode {
	// セルの番号
	int32_t cellID = ID_NONE;

	// 親ノード
	std::shared_ptr<NavNode> parent = nullptr;

	// ノードの状態
	NodeState nodeState = None;

	float aCost = 0.0f;// 実コスト
	float hCost = 0.0f;// 推定コスト
	float score = 0.0f;// スコア

	NavNode() {}
	NavNode(int32_t inCellID) : cellID(inCellID) {}
	~NavNode() {}

	// トータルスコアを取得
	float GetTotalScore() const { return aCost + hCost; }
};

class NavMeshPathfinder {
#pragma region メンバ変数
private:
	std::vector<NavNode> nodes_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// ルート探索
	//void SearchRoute(std::vector<std::shared_ptr<NavCell>> navCells, int32_t startCellID, int32_t endCellID);
#pragma endregion
};

//class NavMeshPathfinder {
//public:
//	struct NavNode {
//		// セルの番号
//		int32_t cellIndex = 0;
//
//		// 親ノード
//		std::shared_ptr<NavNode> parent = nullptr;
//
//		// ノードの状態
//		NodeState nodeState = None;
//
//		// gCost : スタートノードからこのノードまでの最小コスト
//		// hCost : このノードからゴールまでの推定コスト
//		float gCost = 0.0f, hCost = 0.0f;
//
//		NavNode() {}
//		NavNode(int32_t inCellIndex) : cellIndex(inCellIndex) {}
//		~NavNode() {}
//
//		float GetTotalCost() const { return gCost + hCost; }
//	};
//
//private:
//	std::vector<NavNode> NavHeap_;
//	std::vector<int32_t> arrivalWall_;
//
//public:
//	NavMeshPathfinder() {}
//	~NavMeshPathfinder() {}
//
//	bool FindPath(NavMesh* navMesh, int32_t startCellIndex, const Vector3& startPos, int32_t goalCellIndex, const Vector3& endPos);
//	
//	float CostHeuristic(const Vector3& v0, const Vector3& v1)
//	{
//		return Vector3(v0 - v1).length();
//	}
//};