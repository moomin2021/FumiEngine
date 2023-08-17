#include "Bullet.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

Bullet::Bullet(Model* model, BulletType type, const float3& iniPos, const Vector3& moveVec)
{
	// オブジェクト生成＆生成
	data_.object = std::make_unique<Object3D>(model);

	// 生成された時間を記録
	generatedTime_ = Util::GetTimeSec();

	// プレイヤー
	if (type == PLAYER) {
		data_.object->SetPosition(iniPos);
		data_.object->SetScale({ 0.1f, 0.1f, 0.1f });
		data_.moveVec = moveVec;
		data_.bulletSpd = 30.0f;
		data_.aliveTime = 3;
	}

	else if (type == ENEMY0) {
		data_.object->SetPosition(iniPos);
		data_.object->SetScale({ 0.5f, 0.5f, 0.5f });
		data_.object->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		data_.col = std::make_unique<SphereCollider>();
		data_.col->SetAttribute(COL_ENEMY_BULLET);
		data_.col->SetRadius(0.5f);
		data_.col->LinkObject3D(data_.object.get());
		CollisionManager::GetInstance()->AddCollider(data_.col.get());
		data_.moveVec = moveVec;
		data_.bulletSpd = 2.0f;
		data_.aliveTime = 30;
	}

	data_.moveVec.normalize();
}

Bullet::~Bullet()
{
	CollisionManager::GetInstance()->RemoveCollider(data_.col.get());
}

void Bullet::Update()
{
	// 生存フラグが[OFF]ならこの後の処理を飛ばす
	if (isAlive_ == false) return;

	// 弾を移動させる
	data_.object->SetPosition(data_.object->GetPosition() + data_.moveVec * data_.bulletSpd);

	// オブジェクトの更新処理
	data_.object->Update();

	// 生成されてからの経過時間
	uint64_t elapsedTime = Util::GetTimeSec() - generatedTime_;

	// 経過時間が指定の時間を過ぎたら生存フラグを[OFF]にする
	if (elapsedTime >= data_.aliveTime) {
		isAlive_ = false;
	}
}

void Bullet::Draw()
{
	// オブジェクトの描画処理
	data_.object->Draw();
}