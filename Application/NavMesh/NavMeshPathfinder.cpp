#include "NavMeshPathfinder.h"

bool NavMeshPathfinder::FindPath(int32_t startCellID, const Vector3& startPos, int32_t goalCellID, const Vector3& goalPos)
{
	
}

float NavMeshPathfinder::CostHeuristic(const Vector3& v0, const Vector3& v1)
{
	return Vector3(v0 - v1).length();
}