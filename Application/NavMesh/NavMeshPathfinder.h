#pragma once
#define ID_NONE -1

#include "Vector3.h"

#include <vector>
#include <memory>

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

	// gCost : スタートノードからこのノードまでの最小コスト
	// hCost : このノードからゴールまでの推定コスト
	float gCost = 0.0f, hCost = 0.0f;

	NavNode() {}
	NavNode(int32_t inCellID) : cellID(inCellID) {}
	~NavNode() {}

	// トータルスコアを取得
	float GetTotalCost() const { return gCost + hCost; }
};

class NavMeshPathfinder
{
#pragma region メンバ変数
private:
	// ノード
	std::vector<NavNode> navHeap_ = {};
	std::vector<int32_t> arrivalWall_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	NavMeshPathfinder() {}
	~NavMeshPathfinder() {}

	bool FindPath(int32_t startCellID, const Vector3& startPos, int32_t goalCellID, const Vector3& goalPos);

private:
	// 推定コストを取得
	float CostHeuristic(const Vector3& v0, const Vector3& v1);
#pragma endregion
};