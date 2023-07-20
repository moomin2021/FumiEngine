#include "BossGenerator.h"
#include "CollisionManager.h"

BossGenerator::BossGenerator() {
#pragma region ジェネレーター
	// モデル
	mGen_ = std::make_unique<Model>();// 生成

	// オブジェクト
	oGen_ = std::make_unique<Object3D>(mGen_.get());// 生成

	// コライダー
	cGen_ = std::make_unique<SphereCollider>();// 生成
	CollisionManager::GetInstance()->AddCollider(cGen_.get());// コライダーを追加
#pragma endregion

	// ボス
	boss_ = std::make_unique<Boss>();
}

void BossGenerator::Update()
{
	// オブジェクト更新処理
	oGen_->Update();
}

void BossGenerator::Draw()
{
	// オブジェクト描画処理
	oGen_->Draw();
}