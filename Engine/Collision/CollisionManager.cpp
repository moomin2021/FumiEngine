#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

CollisionManager* CollisionManager::GetInstance() {
	// インスタンス生成
	static CollisionManager inst;

	// インスタンスを返す
	return &inst;
}

void CollisionManager::CheckAllCollision() {
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
					colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
				}
			}

			// メッシュと球
			else if (colA->GetShapeType() == SHAPE_MASH && colB->GetShapeType() == SHAPE_SPHERE) {
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Vector3 inter;

				if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
				}
			}

			// 球とメッシュ
			else if (colA->GetShapeType() == SHAPE_SPHERE && colB->GetShapeType() == SHAPE_MASH) {
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				Vector3 inter;
				
				if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
				}
			}
		}
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitinfo, float maxDistance)
{
	bool result = false;

	// 走査用のイテレータ
	std::forward_list<BaseCollider*>::iterator it;

	// 今までで最も近いコライダーを記録するためのイテレータ
	std::forward_list<BaseCollider*>::iterator it_hit;

	// 今までで最も近いコライダーの距離を記録する変数
	float distance = maxDistance;

	// 今までで最も近いコライダーとの交点を記録する変数
	Vector3 inter;

	// 全てのコライダーとの総当たりチェック
	it = colliders_.begin();
	for (; it != colliders_.end(); ++it) {
		BaseCollider* colA = *it;

		// 球の場合
		if (colA->GetShapeType() == SHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			Vector3 tempInter;

			// 当たらなければ除外
			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;

			// 距離が最小でなければ除外
			if (tempDistance >= distance) continue;

			// 今まで最も近いので記録を取る
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}

		// メッシュの場合
		else if (colA->GetShapeType() == SHAPE_MASH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			Vector3 tempInter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	// 最終的になにかに当たっていたら結果を書き込む
	if (result && hitinfo) {
		hitinfo->distance = distance;
		hitinfo->inter = inter;
		hitinfo->collider = *it_hit;
		hitinfo->object = hitinfo->collider->GetObject3D();
	}

	return result;
}
