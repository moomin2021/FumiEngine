#include "NavMeshPath.h"

#include "Util.h"

std::vector<Vector3> NavMeshPath::GetStraightPath(float cornerOffSetRatio)
{
	// コーナーから内側へずらす距離の場合 (0.5fでポータルエッジの中心を通る)
	// 0の場合はきれいに壁に沿った経路を取るが適切な値に設定すれば壁から少し離れた現実的な経路を取る
	std::vector<Vector3> result = {};

	// cornerOffSetRatioを0.0f ~ 1.0fの範囲で収める
	cornerOffSetRatio = Util::Clamp(cornerOffSetRatio, 1.0f, 0.0f);

	std::vector<CellEdge> portalEdges;

	// 先頭のウェイポイントにはEdge情報が無いため先頭の位置をそのままEdgeのLeftとRightとする.
	Vector3 left, right;
	left = right = wayPoints_[0].location;
	portalEdges.emplace_back(CellEdge(left, right));

	for (int32_t i = 1; i < wayPoints_.size() - 1; ++i)
	{
		left = wayPoints_[i].arrivalEdge.left;
		right = wayPoints_[i].arrivalEdge.right;
		portalEdges.emplace_back(CellEdge(left, right));
	}

	// 末尾のウェイポイントにはEdge情報が無いため先頭の位置をそのままEdgeのLeftとRightとする.
	left = right = wayPoints_[wayPoints_.size() - 1].location;
	portalEdges.emplace_back(CellEdge(left, right));


	// ファンネルアルゴリズムを開始するポータルエッジの基準点と基準点から見て左側の点と右側の点
	Vector3 portalApex, portalLeft, portalRight;
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;

	// ポータルは三角形
	portalApex = portalEdges[0].left;	// ポータルの頂点
	portalLeft = portalEdges[0].left;	// ポータルエッジの左側の点
	portalRight = portalEdges[0].right;	// ポータルエッジの右側の点

	result.emplace_back(portalApex);

	for (int i = 1; i < portalEdges.size(); i++)
	{
		Vector3 nextLeft = portalEdges[i].left;		// 次のポータルエッジの左の候補点
		Vector3 nextRight = portalEdges[i].right;	// 次のポータルエッジの右の候補点


		// 右の点を更新

		// NextRight が PortalRight よりも左側にあるということはファンネルをより狭く出来るということなので PortalRight を Right へと進める.
 		PointSide Classification = ClassifyPoint(portalApex, portalRight, nextRight);
		if (Classification != PointSide::RIGHT_SIDE)
		{
			// NextRight は PortalApex と PortalLeft からなるベクトルよりも右側にあるかチェック（正しい三角形が構成されているかチェック）
			Classification = ClassifyPoint(portalApex, portalLeft, nextRight);
			if (portalApex == portalRight || Classification == PointSide::RIGHT_SIDE)
			{
				// ファンネルを次の右の位置に進める
				portalRight = nextRight;
				rightIndex = i;
			}
			// NextRight が PortalLeft を追い越してしまった
			else
			{
				portalLeft = (1 - cornerOffSetRatio) * portalLeft + (cornerOffSetRatio * portalEdges[leftIndex].right);
				result.emplace_back(portalLeft);

				// 左の点からポータルの頂点を作成
				portalApex = portalLeft;
				apexIndex = leftIndex;
				// ポータルをリセット
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				// Restart scan
				i = apexIndex;
				continue;
			}
		}

		// 左の点を更新
		// やっていることは右の点と同じ

		Classification = ClassifyPoint(portalApex, portalLeft, nextLeft);
		if (Classification != PointSide::LEFT_SIDE)
		{
			Classification = ClassifyPoint(portalApex, portalRight, nextLeft);
			if (portalApex == portalLeft || Classification == PointSide::LEFT_SIDE)
			{
				portalLeft = nextLeft;
				leftIndex = i;
			}
			else
			{
				portalRight = (1 - cornerOffSetRatio) * portalRight + (cornerOffSetRatio * portalEdges[rightIndex].left);
				result.emplace_back(portalRight);

				portalApex = portalRight;
				apexIndex = rightIndex;
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = apexIndex;
				continue;
			}
		}
	}

	if (result.size() == 0 || result[result.size() - 1] != portalEdges[portalEdges.size() - 1].left)
	{
		result.emplace_back(wayPoints_[wayPoints_.size() - 1].location);
	}

	std::vector<Vector3> outputVec = result;

	return outputVec;
}

void NavMeshPath::AddWayPoint(const Vector3& inLocation)
{
	wayPoints_.emplace_back(NavWayPoint(inLocation));
}

void NavMeshPath::AddWayPoint(NavCell* pCell, NavCell* pParent)
{
	wayPoints_.emplace_back(NavWayPoint(pCell->GetCenter(), pCell->GetCellID(), pCell->GetArrivalEdge(pParent->GetCellID())));
}

void NavMeshPath::AddWayPoint(const NavWayPoint& inNavWayPoint)
{
	wayPoints_.emplace_back(NavWayPoint(inNavWayPoint.location, inNavWayPoint.cellIndex, inNavWayPoint.arrivalEdge));
}

std::vector<Vector3> NavMeshPath::GetWayPoints()
{
	std::vector<Vector3> result = {};

	for (auto& it : wayPoints_)
	{
		result.emplace_back(it.location);
	}

	return result;
}

NavMeshPath::PointSide NavMeshPath::ClassifyPoint(const Vector3& a, const Vector3& b, const Vector3& c)
{
	Vector2 ab = Vector2(b.x, b.z) - Vector2(a.x, a.z);
	Vector2 ac = Vector2(c.x, c.z) - Vector2(a.x, a.z);

	float value = ac.normalize().cross(ab.normalize());

	if (value == 0) return PointSide::ON_LINE;
	if (value < 0) return PointSide::LEFT_SIDE;
	else return PointSide::RIGHT_SIDE;
}