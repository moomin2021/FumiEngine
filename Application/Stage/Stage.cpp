#include "Stage.h"
#include "CollisionAttribute.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

Stage::Stage() {}

Stage::~Stage() {
	for (auto& it : colliders_) {
		colMgr_->RemoveCollider(it.get());
	}
}

void Stage::Initialize()
{
	// 衝突判定管理クラスインスタンス
	colMgr_ = CollisionManager::GetInstance();

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
	for (auto& object : objects_) object->MatUpdate();

	// スカイドーム
	oSkydome_->MatUpdate();
}

void Stage::Draw()
{
	// オブジェクト描画処理
	for (auto& object : objects_) object->Draw();

	// スカイドーム
	oSkydome_->Draw();
}

void Stage::Load(std::string fileName, bool isCol)
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
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
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

			// visible(表示批評)
			//objectData.isActive = object["visible"];

			// ｔランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			// 平行移動
			objectData.translation.x = -(float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			// 回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];

			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];
		}

		// オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		if (objectData.className == "Enemy") {
			if (isCol == false) continue;
			// 敵を追加
			//enemyMgr_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else if (objectData.className == "Item") {
			if (isCol == false) continue;
			// アイテムを追加
			ItemManager::GetInstace()->AddItem(objectData.translation);
		}

		else if (objectData.className == "BossGenerator") {
			if (isCol == false) continue;
			enemyMgr_->SetBossGenerator(objectData.translation);
		}

		// オブジェクト追加
		else {
			// オブジェクト
			objects_.emplace_front(std::make_unique<Object3D>(models_[objectData.fileName].get()));
			objects_.front()->SetPosition(objectData.translation);
			objects_.front()->SetRotation(objectData.rotation);
			objects_.front()->SetScale(objectData.scaling);

			if (isCol == false) continue;

			// コライダー
			colliders_.emplace_front(std::make_unique<AABBCollider>());
			colliders_.front()->SetRadius(Vector3(0.5f, 0.5f, 0.5f));
			colliders_.front()->SetAttribute(COL_STAGE_OBJ);
			colliders_.front()->SetObject3D(objects_.front().get());
			colMgr_->AddCollider(colliders_.front().get());
		}
	}
}
