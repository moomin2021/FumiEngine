#include "NavCell.h"

NavCell::NavCell(int32_t inCellID, const Vector3& pA, const Vector3& pB, const Vector3& pC)
{
	// セルの番号を登録
	cellID_ = inCellID;

	// 各コンテナの要素数指定
	vertices_.resize(3);
	edges_.resize(3);
	linkID_.resize(3);

	// メッシュ3点の座標
	vertices_[(uint8_t)CellVert::VERT_A] = pA;
	vertices_[(uint8_t)CellVert::VERT_B] = pB;
	vertices_[(uint8_t)CellVert::VERT_C] = pC;

	// 3辺の情報
	edges_[(uint8_t)CellSide::SIDE_AB] = CellEdge(pA, pB);
	edges_[(uint8_t)CellSide::SIDE_BC] = CellEdge(pB, pC);
	edges_[(uint8_t)CellSide::SIDE_CA] = CellEdge(pC, pA);

	// 辺にリンクするセルID
	linkID_[(uint8_t)CellSide::SIDE_AB] = ID_NONE;
	linkID_[(uint8_t)CellSide::SIDE_BC] = ID_NONE;
	linkID_[(uint8_t)CellSide::SIDE_CA] = ID_NONE;

	// メッシュの中心座標と法線方向の計算
	center_ = (pA + pB + pC) / 3.0f;
	normal_ = Vector3(pB - pA).cross(pC - pA);
	normal_.normalize();
}

uint8_t NavCell::ValidLinkCount()
{
	uint8_t count = 0;
	count += (linkID_[0] != -1);
	count += (linkID_[1] != -1);
	count += (linkID_[2] != -1);
	return count;
}

bool NavCell::CheckSharedEdge(const CellEdge& edge, CellSide& sideType)
{
	// 始点と頂点座標を比較して辺のタイプを調べる
	if (vertices_[(uint8_t)CellVert::VERT_A] == edge.left)
	{
		if (vertices_[(uint8_t)CellVert::VERT_B] == edge.right)
		{
			sideType = CellSide::SIDE_AB;
			return true;
		}
		else if (vertices_[(uint8_t)CellVert::VERT_C] == edge.right)
		{
			sideType = CellSide::SIDE_CA;
			return true;
		}
	}

	if (vertices_[(uint8_t)CellVert::VERT_B] == edge.left)
	{
		if (vertices_[(uint8_t)CellVert::VERT_A] == edge.right)
		{
			sideType = CellSide::SIDE_AB;
			return true;
		}
		else if (vertices_[(uint8_t)CellVert::VERT_C] == edge.right)
		{
			sideType = CellSide::SIDE_BC;
			return true;
		}
	}

	if (vertices_[(uint8_t)CellVert::VERT_C] == edge.left)
	{
		if (vertices_[(uint8_t)CellVert::VERT_A] == edge.right)
		{
			sideType = CellSide::SIDE_CA;
			return true;
		}
		else if (vertices_[(uint8_t)CellVert::VERT_B] == edge.right)
		{
			sideType = CellSide::SIDE_BC;
			return true;
		}
	}

	return false;
}

bool NavCell::AlreadyLinkCell(int32_t inCellID)
{
	bool result = false;

	for (size_t i = 0; i < 3; i++)
	{
		if (linkID_[i] == inCellID) result = true;
	}

	return result;
}
