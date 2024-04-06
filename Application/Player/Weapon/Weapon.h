#pragma once
#include "Model.h"
#include "Object3D.h"

#include <memory>

class Weapon
{
#pragma region メンバ変数
protected:
	// モデルとオブジェクト
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Object3D> object_ = nullptr;

	Object3D* parent_ = nullptr;

	// その他情報
	uint16_t maxBullet_ = 0;
	uint16_t nowBullet_ = 0;
	float shotInterval_ = 0.0f;

	bool isAds_ = false;
	float adsRate_ = 0.0f;
	float adsRateIncrDecValue_ = 0.0f;
	const float adsLimit_ = 1.0f;

	bool isReload_ = false;
	float reloadTime_ = 0.0f;
	uint64_t startReloadTime_ = 0;

	Vector3 adsOffset_ = Vector3();
	Vector3 noAdsOffset_ = Vector3();
	Vector3 position_ = Vector3();

#pragma endregion

#pragma region メンバ関数
public:
	Weapon(Object3D* inObject) : parent_(inObject) {}
	virtual ~Weapon() {};

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Collision() = 0;
	virtual void MatUpdate() = 0;
	virtual void Finalize() = 0;

	void PosRotaUpdate(const Vector3& forwardVec, const Vector3& rightVec, const Vector3& eyeAngle);
	void StartReload();
	bool Shot();
	void Pitching(bool isMove);
	
protected:
	void AdsProcess();
	void ReloadProcess();
#pragma endregion

#pragma region セッター関数
public:
	void SetADS(bool flag) { isAds_ = flag; }
#pragma endregion

#pragma region ゲッター関数
public:
	uint16_t GetMaxBullet() { return maxBullet_; }
	uint16_t GetNowBullet() { return nowBullet_; }
	bool GetIsReload() { return isReload_; }
	float GetADSRate() { return adsRate_; }
	bool GetIsADS() { return isAds_; }
#pragma endregion
};