#include "Weapon.h"

#include "Util.h"
#include "Easing.h"

#include <imgui_impl_DX12.h>

void Weapon::AdsProcess()
{
	// ADS率の変更
	if (isAds_) adsRate_ += adsRateIncrDecValue_;
	else adsRate_ -= adsRateIncrDecValue_;

	// ADS率を(0 ~ 1.0f)までに収める
	adsRate_ = Util::Clamp(adsRate_, adsLimit_, 0.0f);
}

void Weapon::ReloadProcess()
{
	// リロード処理
	if (isReload_ == false) return;

	float elapsedTime = (Util::GetTimrMSec() - startReloadTime_) / 1000.0f;
	
	if (elapsedTime >= reloadTime_)
	{
		isReload_ = false;
		nowBullet_ = maxBullet_;
	}
}

void Weapon::PosRotaUpdate(const Vector3& forwardVec, const Vector3& rightVec, const Vector3& eyeAngle)
{
	// 各座標の計算
	Vector3 resultPos = Vector3();
	Vector3 adsPos = Vector3(0.0f, adsOffset_.y, 0.0f) + (forwardVec * adsOffset_.z);
	Vector3 noAdsPos = Vector3(0.0f, noAdsOffset_.y, 0.0f) + (forwardVec * noAdsOffset_.z) + (rightVec * noAdsOffset_.x);

	// ADS率によって座標を遷移させる
	resultPos.x = Easing::lerp(noAdsPos.x, adsPos.x, adsRate_);
	resultPos.y = Easing::lerp(noAdsPos.y, adsPos.y, adsRate_);
	resultPos.z = Easing::lerp(noAdsPos.z, adsPos.z, adsRate_);

	// オブジェクト座標の更新
	position_ += resultPos;

	// オブジェクト回転の更新
	object_->SetRotation({eyeAngle.y + 90.0f, eyeAngle.x, 180.0f});
}

void Weapon::StartReload()
{
	// リロード開始処理
	if (isReload_) return;

	isReload_ = true;
	startReloadTime_ = Util::GetTimrMSec();
	nowBullet_ = 0;
}

bool Weapon::Shot()
{
	static uint64_t shotTime = 0;

	// 最後に弾を撃ってからの経過時間
	float result = (Util::GetTimrMSec() - shotTime) / 1000.0f;

	if (result < shotInterval_) return false;

	nowBullet_--;
	shotTime = Util::GetTimrMSec();

	if (nowBullet_ <= 0) StartReload();

	return true;
}

void Weapon::Pitching(bool isMove)
{
	static float count = 0;

	if (isMove == false) return;

	count++;

	position_.y += sinf(count / 10.0f) * 0.03f;
}