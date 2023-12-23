#include "Stage.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

Stage::Stage() {}

Stage::~Stage() {
	for (auto& it : colliders_) {
		colMgr_->RemoveBlockCollider(it.get());
	}
}

void Stage::Initialize()
{
	// 衝突判定管理クラスインスタンス
	colMgr_ = CollisionManager::GetInstance();

	// インスタンシング描画初期化
	objects_ = std::make_unique<Instancing3D>((uint16_t)3000);

#pragma region スカイドーム
	// モデル
	mSkydome_ = std::make_unique<Model>("skydome");

	// オブジェクト
	oSkydome_ = std::make_unique<Object3D>(mSkydome_.get());
	oSkydome_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
#pragma endregion
}

void Stage::MatUpdate()
{
	// オブジェクト行列更新
	objects_->MatUpdate();

	// スカイドーム
	oSkydome_->MatUpdate();
}

void Stage::Draw()
{
	PipelineManager::PreDraw("Instancing3D");
	// オブジェクト描画処理
	objects_->Draw();

	PipelineManager::PreDraw("Object3D");
	// スカイドーム
	oSkydome_->Draw();
}

void Stage::Load(std::string fileName, bool isCol, bool isCore)
{
	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fileName);

	// ファイルオープン失敗チェック
	if (file.fail()) assert(0);

	// JSON文字列から回答したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルエディタファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		// 種別を取得
		//std::string type = object["type"].get<std::string>();

		// MESH
		if (true) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});

			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			// モデル
			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];

				// 同じファイル名のモデルが無かったらモデル追加
				if (models_.count(object["file_name"]) == 0) {
					models_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
				}
			}

			// クラスの名前
			if (object.contains("class_name")) {
				objectData.className = object["class_name"];
			}

			// ｔランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			// 平行移動
			objectData.translation.x = -(float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			if (isCol == false) continue;

			// スケーリング
			objectData.scale.x = (float)transform["scaling"][0];
			objectData.scale.y = (float)transform["scaling"][2];
			objectData.scale.z = (float)transform["scaling"][1];
		}
	}

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {

		if (objectData.className == "enemyCore") {
			if (isCore == false) continue;
			enemyMgr_->AddCore(objectData.translation);
		}

		// オブジェクト追加
		else {
			if (isCol == false)
			{
				// オブジェクト
				if (objects_->GetModel() == nullptr) objects_->SetModel(models_[objectData.fileName].get());
				objects_->AddTransform(objectData.translation);
			}

			else
			{
				// コライダー
				colliders_.emplace_front(std::make_unique<AABBCollider>());
				colliders_.front()->SetOffset(objectData.translation);
				colliders_.front()->SetRadius(objectData.scale / 2.0f);
				colliders_.front()->SetAttribute(COL_STAGE_OBJ);
				colMgr_->AddBlockCollider(colliders_.front().get());
			}
		}
	}
}
