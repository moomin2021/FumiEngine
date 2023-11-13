#pragma once
#include "Vector3.h"

#include "NavCell.h"

class NavMeshPath
{
public:
	enum class PointSide : uint8_t {
		LEFT_SIDE = 0,
		RIGHT_SIDE,
		ON_LINE,
	};

	struct NavWayPoint {
		Vector3 location = { 0.0f, 0.0f, 0.0f };
		int32_t cellIndex = ID_NONE;
		CellEdge arrivalEdge = {};

		NavWayPoint() {}
		NavWayPoint(const Vector3& inLocation) : location(inLocation) {}
		NavWayPoint(const Vector3& inLocation, int32_t inCellIndex, const CellEdge& inEdge) :
			location(inLocation), cellIndex(inCellIndex), arrivalEdge(inEdge) {}
	};

#pragma region メンバ変数
public:
	std::vector<NavWayPoint> wayPoints_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	NavMeshPath() {}
	~NavMeshPath() {}

	std::vector<Vector3> GetStraightPath(float cornerOffSetRatio);

	void AddWayPoint(const Vector3& inLocation);
	void AddWayPoint(NavCell* pCell, NavCell* pParent);
	void AddWayPoint(const NavWayPoint& inNavWayPoint);
	std::vector<NavWayPoint> GetWayPoints() const { return wayPoints_; }
	void Clear() { wayPoints_.clear(); }

private:
	PointSide ClassifyPoint(const Vector3& a, const Vector3& b, const Vector3& c);
#pragma endregion
};