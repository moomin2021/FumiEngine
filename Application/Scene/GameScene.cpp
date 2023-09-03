#include "GameScene.h"
#include "Object3D.h"
#include "PipelineManager.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

GameScene::GameScene() {}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
#pragma region ライト
	// ライトグループ生成
	lightGroup_ = std::make_unique<LightGroup>();

	// 平行光源生成
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });

	// 平行光源を登録
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

	// ステージマネージャー生成
	stageMgr_ = std::make_unique<StageManager>();
	stageMgr_->Initialize();

	// エネミーマネージャー生成
	enemyMgr_ = std::make_unique<EnemyManager>();
	enemyMgr_->Initialize();

	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->Initialize(enemyMgr_.get());

	// オブジェクト3Dにライトを適用
	Object3D::SetLightGroup(lightGroup_.get());

	// ステージ読み込み
	LoadStage("Resources/Stage/stage1.json");
}

void GameScene::Update()
{
	// エネミーマネージャー
	enemyMgr_->Update();

	// プレイヤー
	player_->Update();

	// 衝突時処理
	OnCollision();

	// オブジェクト更新処理
	ObjUpdate();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// ステージマネージャー
	stageMgr_->Draw();

	// エネミーマネージャー
	enemyMgr_->Draw();

	// プレイヤー
	player_->DrawObject3D();

	PipelineManager::PreDraw("Sprite");

	// プレイヤー前面スプライト描画
	player_->DrawSprite();
}

void GameScene::ObjUpdate()
{
	// ステージマネージャー
	stageMgr_->ObjUpdate();

	// エネミーマネージャー
	enemyMgr_->ObjUpdate();

	// プレイヤー
	player_->ObjUpdate();
}

void GameScene::OnCollision()
{
	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();

	// ステージマネージャー
	stageMgr_->OnCollision();

	// エネミーマネージャー
	enemyMgr_->OnCollision();

	// プレイヤー
	player_->OnCollision();
}

void GameScene::LoadStage(std::string fileName)
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

				// モデル追加
				stageMgr_->AddModel(object["file_name"]);
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
			//enemyManager_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else if (objectData.className == "Item") {
			// アイテムを追加
			//ItemManager::GetInstace()->AddItem(objectData.translation);
		}

		else if (objectData.className == "BossGenerator") {
			enemyMgr_->SetBossGenerator(objectData.translation);
		}

		else {
			// オブジェクト追加
			stageMgr_->AddObject3D(objectData.fileName, objectData.translation, objectData.rotation, objectData.scaling);
		}
	}
}