#include "CollisionManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "MeshCollider.h"

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
		BaseCollider* colA = *itA;

		// 衝突判定を取らないかどうか
		if (colA->GetIsCollision() == false) continue;

		// レイだったら
		if (colA->GetShapeType() == SHAPE_RAY) {
			bool result = false;

			// 走査用のイテレータ
			std::forward_list<BaseCollider*>::iterator it;

			// 今までで最も近いコライダーを記録するためのイテレータ
			BaseCollider* it_hit = nullptr;

			// 今までで最も近いコライダーの距離を記録する変数
			float distance = FLT_MAX;

			// 今までで最も近いコライダーとの交点を記録する変数
			Vector3 inter;

			// 全てのコライダーとの総当たりチェック
			it = colliders_.begin();
			for (; it != colliders_.end(); ++it) {
				BaseCollider* col = *it;

				// 衝突判定を取らないかどうか
				if (col->GetIsCollision() == false) continue;

				// 属性が合わなければスキップ
				if (!(colA->attribute_ & col->attribute_)) continue;

				Ray* ray = dynamic_cast<Ray*>(colA);

				// 球の場合
				if (col->GetShapeType() == SHAPE_SPHERE) {
					Sphere* sphere = dynamic_cast<Sphere*>(col);
					float tempDistance;
					Vector3 tempInter;

					// 当たらなければ除外
					if (!Collision::CheckRay2Sphere(*ray, *sphere, &tempDistance, &tempInter)) continue;

					// 距離が最小でなければ除外
					if (tempDistance >= distance) continue;

					// 今まで最も近いので記録を取る
					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}

				// メッシュの場合
				else if (col->GetShapeType() == SHAPE_MESH) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

					float tempDistance;
					Vector3 tempInter;
					if (!meshCollider->CheckCollisionRay(*ray, &tempDistance, &tempInter)) continue;
					if (tempDistance >= distance) continue;

					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}
			}

			// 当たっていたら
			if (result) {
				colA->SetIsHit(true);
				colA->SetInter(inter);
				it_hit->SetIsHit(true);
				it_hit->SetInter(inter);
			}
		}

		else {
			for (; itB != colliders_.end(); ++itB) {
				BaseCollider* colB = *itB;

				// 衝突判定を取らないかどうか
				if (colB->GetIsCollision() == false) continue;

				// 属性が合わなければスキップ
				if (!(colA->attribute_ & colB->attribute_)) continue;

				// ともに球
				if (colA->GetShapeType() == SHAPE_SPHERE && colB->GetShapeType() == SHAPE_SPHERE) {
					Sphere* sphereA = dynamic_cast<Sphere*>(colA);
					Sphere* sphereB = dynamic_cast<Sphere*>(colB);
					Vector3 inter;
					if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colB->SetIsHit(true);
						colB->SetInter(inter);
					}
				}

				// メッシュと球
				else if (colA->GetShapeType() == SHAPE_MESH && colB->GetShapeType() == SHAPE_SPHERE) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					Vector3 inter;
					Vector3 reject;

					if (meshCollider->CheckCollisionSphere(*sphere, &inter, &reject)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colA->SetReject(reject);
						colA->SetCollider(colB);
						colB->SetIsHit(true);
						colB->SetInter(inter);
						colB->SetReject(reject);
						colB->SetCollider(colA);
					}
				}

				// 球とメッシュ
				else if (colA->GetShapeType() == SHAPE_SPHERE && colB->GetShapeType() == SHAPE_MESH) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					Vector3 inter;
					Vector3 reject;

					if (meshCollider->CheckCollisionSphere(*sphere, &inter, &reject)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colA->SetReject(reject);
						colA->SetCollider(colB);
						colB->SetIsHit(true);
						colB->SetInter(inter);
						colB->SetReject(reject);
						colB->SetCollider(colA);
					}
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