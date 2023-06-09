#include "CollisionManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"

CollisionManager::~CollisionManager()
{
	// コライダー削除
	colliders_.clear();
}

void CollisionManager::CheckAllCollision()
{
	for (std::forward_list<BaseCollider*>::iterator it = colliders_.begin(); it != colliders_.end(); ++it)
	{
		BaseCollider* collider = *it;
		collider->Update();
	}

	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// 全ての組み合わせについて総当たりチェック
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			// ともに球
			if (colA->GetShapeType() == SHAPE_SPHERE && colA->GetShapeType() == SHAPE_SPHERE) {
				Sphere* sphereA = dynamic_cast<Sphere*>(colA);
				Sphere* sphereB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;
				if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
					colA->SetIsHit(true);
					colB->SetIsHit(true);
				}
			}
		}
	}
}

CollisionManager* CollisionManager::GetInstance()
{
	// インスタンス生成
	static CollisionManager inst;

	// インスタンスを返す
	return &inst;
}
