#pragma once
#include "Model.h"
#include "Vector3.h"

#include <vector>

enum class CellVert : uint8_t {
	VERT_A = 0,
	VERT_B = 1,
	VERT_C = 2,
};

enum class CellSide : uint8_t {
	SIDE_AB = 0,
	SIDE_BC = 1,
	SIDE_CA = 2,
};

class NavMesh
{
#pragma region メンバ変数
private:
	// 頂点データ
	std::vector<Vector3> vertices_ = {};

	// 頂点インデックス
	std::vector<Vector3> indexes_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// メッシュを読み込む
	void LoadMesh(std::string name);
#pragma endregion
};

struct CellEdge {
	Vector3 left = { 0.0f, 0.0f, 0.0f };
	Vector3 right = { 0.0f, 0.0f, 0.0f };
	CellEdge() {}
	CellEdge(const Vector3& inLeft, const Vector3& inRight) : left(inLeft), right(inRight) {}
};

struct Cell {
	int32_t cellID = 0;
	std::vector<Vector3> vertices;
	std::vector<CellEdge> edges;
	std::vector<int32_t> link;
	Vector3 center = { 0.0f, 0.0f, 0.0f };
	Vector3 normal = { 0.0f, 0.0f, 0.0f };
	Cell(int32_t aCellID, const Vector3& pointA, const Vector3& pointB, const Vector3& pointC);
	bool AlreadyLinkCell(int32_t inCellID);
	uint8_t ValidLinkCount();
	void SetLink(CellSide sideType, int32_t other) { link[(uint8_t)sideType] = other; }
	int32_t GetLink(CellSide sideType) { return link[(uint8_t)sideType]; }
	const CellEdge& GetEdge(CellSide sideType) const { return edges[(uint8_t)sideType]; }
	bool CheckSharedEdge(const CellEdge& Edge, CellSide& SideType);
};