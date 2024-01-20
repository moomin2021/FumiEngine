#include "RayCollider.h"

RayCollider::RayCollider(Vector3 offset, Vector3 dir)
{
	shapeType_ = SHAPE_RAY;

	offset_ = offset;

	Ray::dir = dir;
}

void RayCollider::Update()
{
	// オブジェクト3Dが紐づけられていたら
	if (object_) {
		// ワールド行列からワールド座標を抽出
		const Vector3 objPos = object_->GetPosition();
		Ray::start = objPos + offset_;
	}

	else {
		Ray::start = offset_;
	}
}

void RayCollider::Reset()
{
	// 衝突フラグを初期化
	isHit_ = false;

	// 衝突したときの情報を初期化
	inter_ = { 0.0f, 0.0f, 0.0f };
	minDistance_ = FLT_MAX;
}

void RayCollider::PushBack()
{
	// オブジェクト3Dが紐づけられていたら
	if (object_)
	{
		// 接点からレイ開始位置までのベクトル
		Vector3 pushBack = Ray::start - inter_;

		// レイの開始位置から接点までの距離が押し戻し距離より長かったら処理を飛ばす
		if (pushBack.length() >= pushBackDis_) return;

		// 押し戻し距離との差分を計算
		float diff = pushBackDis_ - pushBack.length();

		// 押し戻しベクトルの正規化
		pushBack.normalize();

		// 押し戻すベクトルを計算
		pushBack = pushBack * diff;

		// 押し戻す
		object_->SetPosition(object_->GetPosition() + pushBack);
		Ray::start = object_->GetPosition() + offset_;
	}

	//// 衝突フラグを初期化
	//isHit_ = false;

	//// 衝突したときの情報を初期化
	//inter_ = { 0.0f, 0.0f, 0.0f };
	//minDistance_ = FLT_MAX;
}