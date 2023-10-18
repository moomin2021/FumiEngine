#pragma once
#include "Vector3.h"
#include "CollisionPrimitive.h"

#include <vector>

#define ID_NONE -1

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

// 辺の情報
struct CellEdge {
	Vector3 left = { 0.0f, 0.0f, 0.0f };
	Vector3 right = { 0.0f, 0.0f, 0.0f };
	CellEdge() {}
	CellEdge(const Vector3& inLeft, const Vector3& inRight) : left(inLeft), right(inRight) {}
};

class NavCell
{
#pragma region メンバ変数
private:
	// セルID
	int32_t cellID_ = ID_NONE;

	std::vector<Vector3> vertices_	= {};// メッシュの3点座標
	std::vector<CellEdge> edges_	= {};// 3辺の方向
	std::vector<int32_t> linkID_	= {};// リンクしてるセルID

	Vector3 center_ = { 0.0f, 0.0f, 0.0f };// メッシュの中心座標
	Vector3 normal_ = { 0.0f, 0.0f, 0.0f };// メッシュの法線方向

	// 当たり判定用の三角形の情報
	Triangle triangle_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="inCellID"> セルID </param>
	/// <param name="pA"> メッシュにある1点の座標 </param>
	/// <param name="pB"> メッシュにある1点の座標 </param>
	/// <param name="pC"> メッシュにある1点の座標 </param>
	NavCell(int32_t inCellID, const Vector3& pA, const Vector3& pB, const Vector3& pC);

	// リンクしている数を取得
	uint8_t LinkCount();

	// 受け取ったエッジがどの辺のタイプか調べる
	bool CheckSharedEdge(const CellEdge& edge, CellSide& sideType);

	// 指定したセルがリンク済みかどうか
	bool AlreadyLinkCell(int32_t inCellID);
#pragma endregion

#pragma region セッター関数
public:
	// リンクIDを設定
	void SetLinkID(CellSide sideType, int32_t inCellID) { linkID_[(uint8_t)sideType] = inCellID; }
#pragma endregion

#pragma region ゲッター関数
public:
	// セルIDを取得
	int32_t GetCellID() { return cellID_; }

	// 指定する辺の情報を取得
	const CellEdge& GetEdge(CellSide sideType) const { return edges_[(uint8_t)sideType]; }

	// リンクIDを取得
	int32_t GetLinkID(CellSide sideType) { return linkID_[(uint8_t)sideType]; }
	std::vector<int32_t> GetLinkID() const { return linkID_; }

	// メッシュの中心座標を取得
	const Vector3& GetCenter() { return center_; }
#pragma endregion

#pragma region オペレーター
public:
	bool operator==(const NavCell* other) const { return cellID_ == other->cellID_; }
	bool operator==(int32_t id) const { return cellID_ == id; }
#pragma endregion
};