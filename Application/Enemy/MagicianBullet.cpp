#include "MagicianBullet.h"
#include "PipelineManager.h"
#include "CollisionAttribute.h"
#include "Util.h"

CollisionManager* MagicianBullet::sColMgr_ = nullptr;
uint16_t MagicianBullet::sParticleHandle_ = 0;
Player* MagicianBullet::sPlayer_ = nullptr;

void MagicianBullet::Initialize(const Vector3& startPos, const Vector3& inDir)
{
	// 座標の設定
	pos_ = startPos;
	dir_ = inDir;

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>();

	// コライダー初期化
	collider_ = std::make_unique<AABBCollider>();
	collider_->SetOffset(pos_);
	collider_->SetRadius({ radius_, radius_, radius_ });
	collider_->SetAttribute(COL_ENEMY_BULLET);
	sColMgr_->AddCollider(collider_.get());
}

void MagicianBullet::Update()
{
	// 座標を更新
	pos_ += dir_ * speed_;
	particleEmitter_->SetSpawnPos(pos_);
	collider_->SetOffset(pos_);

	// パーティクルを生成
	uint16_t generateNum = Util::GetRandomInt(minOneTimeGenerate_, maxOneTimeGenerate_);
	for (uint16_t i = 0; i < generateNum; i++) CreateParticle();
}

void MagicianBullet::Draw()
{
	particleEmitter_->Draw(sParticleHandle_);
}

void MagicianBullet::OnCollision()
{
	// コライダーがヒットしていなかったら処理を飛ばす
	if (collider_->GetIsHit() == false) return;

	// 衝突した属性によって処理を変える
	HitStageObj();
	HitPlayer();
}

void MagicianBullet::MatUpdate()
{
	particleEmitter_->Update(BILLBOARD::Y);
}

MagicianBullet::~MagicianBullet()
{
	sColMgr_->RemoveCollider(collider_.get());
}

void MagicianBullet::CreateParticle()
{
	// 位置をランダムにする
	Vector3 pos = { 0.0f, 0.0f, 0.0f };
	pos.x = Util::GetRandomFloat(-radius_, radius_);
	pos.y = Util::GetRandomFloat(-radius_, radius_);
	pos.z = Util::GetRandomFloat(-radius_, radius_);
	particleEmitter_->Add(particleLife_, pos, -dir_ * particleSpeed_, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 0.0f);
}

void MagicianBullet::HitStageObj()
{
	// 衝突したのがステージのオブジェクトではなかったら処理を飛ばす
	if (collider_->GetHitCollider()->GetAttribute() != COL_STAGE_OBJ) return;

	// 生存フラグを[OFF]にする
	isAlive_ = false;
}

void MagicianBullet::HitPlayer()
{
	// 衝突したのがプレイヤーではなかったら処理を飛ばす
	if (collider_->GetHitCollider()->GetAttribute() != COL_PLAYER) return;

	// 弾の進行方向を設定しプレイヤーをノックバックさせる
	sPlayer_->SetKnock(dir_);
}