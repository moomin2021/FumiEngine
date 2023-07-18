#include "LoadStage.h"
#include "EnemyManager.h"
#include "Enemy0.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

LoadStage::LoadStage()
{
	mEnemy_ = std::make_unique<Model>("cube");
	LoadJson("Resources/Stage/stage1.json");
}

void LoadStage::Update()
{
	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	// 弾の更新処理
	//	enemys_[i]->Update();

	//	// 生存フラグが[OFF]だったら
	//	if (enemys_[i]->GetIsAlive() == false) {
	//		// 弾を消す
	//		enemys_.erase(enemys_.begin() + i);
	//	}
	//}
	for (auto& objs : object_) {
		objs->Update();
	}

}

void LoadStage::Draw()
{
	for (auto& objs : object_) {
		objs->Draw();
	}

	//for (auto& enemys : enemys_) {
	//	enemys->Draw();
	//}
}

void LoadStage::LoadJson(std::string fileName)
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

				if (model_.count(object["file_name"]) == 0) {
					model_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
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
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			// 回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
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
			//std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>();
			Enemy0* newEnemy = new Enemy0(mEnemy_.get());

			// 初期化
			newEnemy->Initialize(objectData.translation);

			// 配列に登録
			EnemyManager::GetInstance()->AddEnemy(newEnemy);
			//enemys_.emplace_back(std::move(newEnemy));
		}

		else {
			// モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>(model_[objectData.fileName].get());

			// 座標
			newObject->SetPosition(objectData.translation);

			// 回転角
			newObject->SetRotation(objectData.rotation);

			// 拡縮
			newObject->SetScale(objectData.scaling);

			// 表示非表示
			//newObject->SetActive(objectData.isActive);

			// 配列に登録
			object_.emplace_back(std::move(newObject));
		}
	}
}