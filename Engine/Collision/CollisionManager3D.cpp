#include "CollisionManager3D.h"
#include "CollisionAttribute.h"
#include "Collision.h"

#include <cassert>

void CollisionManager3D::CheckAllCollision()
{
	// コライダーの更新
	ColliderUpdate();

	// 押し戻し用レイとブロックの衝突判定処理
	PushBackRayAndBlockCollision();

	// AAB同士の衝突判定処理
	AABBCollision();

	// ブロックとAABBの衝突判定処理
	BlockAndAABBCollision();

	// レイとその他のコライダーとの衝突判定処理
	RayAndOthersCollision();
}

void CollisionManager3D::AddCollider(BaseCollider* collider)
{
	HRESULT result = FALSE;

	// コライダーの形状タイプがAABBなら
	if (collider->GetShapeType() == SHAPE_AABB)
	{
		AABBCollider* aabbCollider = dynamic_cast<AABBCollider*>(collider);
		aabbColliders_.emplace_front(aabbCollider);
		result = S_OK;
	}

	// コライダーの形状タイプがRayなら
	else if (collider->GetShapeType() == SHAPE_RAY)
	{
		RayCollider* rayCollider = dynamic_cast<RayCollider*>(collider);
		rayColliders_.emplace_front(rayCollider);
		result = S_OK;
	}

	assert(SUCCEEDED(result));
}

void CollisionManager3D::RemoveCollider(BaseCollider* collider)
{
	HRESULT result = FALSE;

	// コライダーの形状タイプがAABBなら
	if (collider->GetShapeType() == SHAPE_AABB)
	{
		AABBCollider* aabbCollider = dynamic_cast<AABBCollider*>(collider);
		aabbColliders_.remove(aabbCollider);
		result = S_OK;
	}

	// コライダーの形状タイプがRayなら
	else if (collider->GetShapeType() == SHAPE_RAY)
	{
		RayCollider* rayCollider = dynamic_cast<RayCollider*>(collider);
		rayColliders_.remove(rayCollider);
		result = S_OK;
	}

	assert(SUCCEEDED(result));
}

void CollisionManager3D::Release()
{
	// 各コライダーの登録解除
	blockColliders_.clear();
	aabbColliders_.clear();
	rayColliders_.clear();
}

CollisionManager3D::~CollisionManager3D()
{
	// 各コライダーの登録解除
	blockColliders_.clear();
	aabbColliders_.clear();
	rayColliders_.clear();
}

void CollisionManager3D::ColliderUpdate()
{
	for (auto& it : pushBackRayColliders_) it->Reset();
	for (auto& it : blockColliders_) it->Reset();
	for (auto& it : aabbColliders_) it->Reset();
	for (auto& it : rayColliders_) it->Reset();
}

void CollisionManager3D::PushBackRayAndBlockCollision()
{
	// 押し戻し用RayとAABBのコライダーと総当たり
	for (auto& itA : pushBackRayColliders_)
	{
		// 衝突フラグ
		bool result = false;

		// 最も近いコライダーの情報
		float distance = FLT_MAX;		// 最も最小の距離
		Vector3 inter = Vector3();		// 最も近い距離にいるコライダーとの交点
		AABBCollider* it_hit = nullptr;	// 最も近いコライダー

		// ブロックの属性と合っていたらブロックと判定を取る
		if (itA->GetAttribute() & COL_BLOCK)
		{
			// ブロックのコライダー
			for (auto& itB : blockColliders_)
			{
				// 判定したときのデータ
				float tempDistance = FLT_MAX;
				Vector3 tempInter = Vector3();

				// コライダーの更新
				itA->Update();
				itB->Update();

				// 衝突していなかったら処理を飛ばす
				if (!Collision::CheckRay2AABB(*itA, *itB, &tempDistance, &tempInter)) continue;

				// 距離が最小でなければ処理を飛ばす
				if (tempDistance >= distance) continue;

				// 最も近いコライダーなので情報を保存
				result = true;
				distance = tempDistance;
				inter = tempInter;
				it_hit = itB;
			}
		}

		// 衝突していなかったら処理を飛ばす
		if (result == false) continue;

		// 衝突フラグを[TRUE]にする
		if (itA->GetPushBackDis() > distance) itA->SetIsHit(true);
		it_hit->SetIsHit(true);

		// お互いを衝突したコライダーとして登録する
		itA->SetHitCollider(it_hit);
		it_hit->SetHitCollider(itA);

		// 情報を設定
		itA->SetInter(inter);
		itA->SetDistance(distance);

		// 押し戻す
		itA->PushBack();
	}
}

void CollisionManager3D::AABBCollision()
{
	// 総当たりするために用意
	auto itA = aabbColliders_.begin();
	auto itB = aabbColliders_.begin();

	// 総当たり
	for (; itA != aabbColliders_.end(); ++itA)
	{
		// イテレーターを進める
		itB = itA;
		++itB;

		for (; itB != aabbColliders_.end(); ++itB)
		{
			// イテレーターが最後だったら処理を飛ばす
			if (itB == aabbColliders_.end()) continue;

			// 属性が合わなければ処理を飛ばす
			if (!((*itA)->GetAttribute() & (*itB)->GetAttribute())) continue;

			// 押し戻しベクトル
			Vector3 pushBackVec = Vector3();

			// コライダーの更新
			(*itA)->Update();
			(*itB)->Update();

			if (Collision::CheckAABB2AABB(*(*itA), *(*itB), &pushBackVec))
			{
				// 衝突フラグを[TRUE]にする
				(*itA)->SetIsHit(true);
				(*itB)->SetIsHit(true);

				// お互いを衝突したコライダーとして登録する
				(*itA)->SetHitCollider(*itB);
				(*itB)->SetHitCollider(*itA);

				// 押し戻し処理
				(*itA)->PushBack(pushBackVec * (*itB)->GetPushBackRate());
				(*itB)->PushBack(-pushBackVec * (*itA)->GetPushBackRate());
			}
		}
	}
}

void CollisionManager3D::BlockAndAABBCollision()
{
	// ブロックとAABBのコライダーの総当たり
	for (auto& itA : aabbColliders_)
	{
		// ブロックの属性と合っていなかったら処理を飛ばす
		if (!(itA->GetAttribute() & COL_BLOCK)) continue;

		for (auto& itB : blockColliders_)
		{
			// 押し戻しベクトル
			Vector3 pushBackVec = Vector3();

			// コライダーの更新
			itA->Update();
			itB->Update();

			// 衝突判定を取り、衝突していなかったら処理を飛ばす
			if (!Collision::CheckAABB2AABB(*itA, *itB, &pushBackVec)) continue;

			// 衝突フラグを[TRUE]にする
			itA->SetIsHit(true);
			itB->SetIsHit(true);

			// お互いを衝突したコライダーとして登録する
			itA->SetHitCollider(itB);
			itB->SetHitCollider(itA);

			// AABBのコライダーを押し戻す
			itA->PushBack(pushBackVec);
		}
	}
}

void CollisionManager3D::RayAndOthersCollision()
{
	// Rayとその他のコライダーと総当たり
	for (auto& itA : rayColliders_)
	{
		// 衝突フラグ
		bool result = false;

		// 最も近いコライダーの情報
		float distance = FLT_MAX;		// 最も最小の距離
		Vector3 inter = Vector3();		// 最も近い距離にいるコライダーとの交点
		AABBCollider* it_hit = nullptr;	// 最も近いコライダー

		// AABBコライダー
		for (auto& itB : aabbColliders_)
		{
			// 判定したときのデータ
			float tempDistance = FLT_MAX;
			Vector3 tempInter = Vector3();

			// 属性が合わなければ処理を飛ばす
			if (!(itA->GetAttribute() & itB->GetAttribute())) continue;

			// コライダーの更新
			itA->Update();
			itB->Update();

			// 衝突していなかったら処理を飛ばす
			if (!Collision::CheckRay2AABB(*itA, *itB, &tempDistance, &tempInter)) continue;

			// 距離が最小でなければ処理を飛ばす
			if (tempDistance >= distance) continue;

			// 最も近いコライダーなので情報を保存
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = itB;
		}

		// ブロックの属性と合っていたらブロックと判定を取る
		if (itA->GetAttribute() & COL_BLOCK)
		{
			// ブロックのコライダー
			for (auto& itB : blockColliders_)
			{
				// 判定したときのデータ
				float tempDistance = FLT_MAX;
				Vector3 tempInter = Vector3();

				// コライダーの更新
				itA->Update();
				itB->Update();

				// 衝突していなかったら処理を飛ばす
				if (!Collision::CheckRay2AABB(*itA, *itB, &tempDistance, &tempInter)) continue;

				// 距離が最小でなければ処理を飛ばす
				if (tempDistance >= distance) continue;

				// 最も近いコライダーなので情報を保存
				result = true;
				distance = tempDistance;
				inter = tempInter;
				it_hit = itB;
			}
		}

		// 衝突していなかったら処理を飛ばす
		if (result == false) continue;

		// 衝突フラグを[TRUE]にする
		itA->SetIsHit(true);
		it_hit->SetIsHit(true);

		// お互いを衝突したコライダーとして登録する
		itA->SetHitCollider(it_hit);
		it_hit->SetHitCollider(itA);

		// 情報を設定
		itA->SetInter(inter);
		itA->SetDistance(distance);
	}
}

CollisionManager3D* CollisionManager3D::GetInstance()
{
	// インスタンス生成
	static CollisionManager3D inst;

	// インスタンスを返す
	return &inst;
}