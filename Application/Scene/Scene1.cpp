#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

using namespace DirectX;

Scene1::Scene1() {}

Scene1::~Scene1() {}

void Scene1::Initialize()
{
#pragma region ライト
	// 平行光源の生成と設定
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	// ライトグループの生成
	lightGroup_ = std::make_unique<LightGroup>();

	// 平行光源をライトグループに登録
	lightGroup_->AddDirLight(dirLight_.get());

	// ライトを適用
	Object3D::SetLightGroup(lightGroup_.get());
#pragma endregion

	// プレイヤー生成
	player_ = std::make_unique<Player>();

	// エネミーマネージャー生成
	enemyManager_ = std::make_unique<EnemyManager>();

	// ステージ読み込み
	LoadStage("Resources/Stage/stage1.json");
}

void Scene1::Update()
{
	// 敵更新
	enemyManager_->Update();

	// ステージ上のオブジェクト更新
	for (auto& i : stageObjects_) i->Update();

	// プレイヤー更新
	player_->Update();

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// 敵描画
	enemyManager_->Draw();

	// ステージ上のオブジェクト描画
	for (auto& i : stageObjects_) i->Draw();

	// プレイヤー描画
	player_->Object3DDraw();

	PipelineManager::PreDraw("Sprite");

	// プレイヤー前面スプライト描画
	player_->FrontSpriteDraw();
}

void Scene1::LoadStage(std::string fileName)
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

				if (stageModels_.count(object["file_name"]) == 0) {
					stageModels_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
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
			// 敵を追加
			enemyManager_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else {
			// モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>(stageModels_[objectData.fileName].get());

			// 座標
			newObject->SetPosition(objectData.translation);

			// 回転角
			newObject->SetRotation(objectData.rotation);

			// 拡縮
			newObject->SetScale(objectData.scaling);

			// 配列に登録
			stageObjects_.emplace_back(std::move(newObject));
		}
	}
}