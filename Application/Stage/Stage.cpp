#include "Stage.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"
#include "Zombie.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

Stage::Stage() {}

Stage::~Stage()
{
	for (auto& it : colliders_)
	{
		colMgr_->RemoveBlockCollider(it.get());
	}
}

void Stage::Initialize()
{
	// 衝突判定管理クラスインスタンス
	colMgr_ = CollisionManager::GetInstance();

	// インスタンシング描画初期化
	objects_ = std::make_unique<Instancing3D>((uint16_t)5000);

#pragma region スカイドーム
	// モデル
	mSkydome_ = std::make_unique<Model>("skydome");

	// オブジェクト
	oSkydome_ = std::make_unique<Object3D>(mSkydome_.get());
	oSkydome_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
#pragma endregion

	// ナビメッシュ
	navMesh_ = std::make_unique<NavMesh>();
	navMesh_->SetIsLinkLineDraw(true);
	Zombie::SetNavMesh(navMesh_.get());
}

void Stage::MatUpdate()
{
	// オブジェクト行列更新
	objects_->MatUpdate();

	// スカイドーム
	oSkydome_->MatUpdate();

	// ナビゲーションメッシュ
	navMesh_->MatUpdate();
}

void Stage::Draw()
{
	PipelineManager::PreDraw("Instancing3D");
	// オブジェクト描画処理
	objects_->Draw();

	PipelineManager::PreDraw("Object3D");
	// スカイドーム
	oSkydome_->Draw();

	navMesh_->Draw();
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
	for (nlohmann::json& object : deserialized["objects"])
	{

		// 種別を取得
		//std::string type = object["type"].get<std::string>();

		// MESH
		if (true)
		{
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});

			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			// モデル
			if (object.contains("file_name"))
			{
				// ファイル名
				objectData.fileName = object["file_name"];

				// 同じファイル名のモデルが無かったらモデル追加
				if (models_.count(object["file_name"]) == 0)
				{
					models_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
				}
			}

			// クラスの名前
			if (object.contains("class_name"))
			{
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
	for (auto& objectData : levelData->objects)
	{

		if (objectData.className == "enemyCore")
		{
			if (isCore == false) continue;
			enemyMgr_->AddCore(objectData.translation);
		}

		// オブジェクト追加
		else
		{
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

void Stage::CreateStage()
{
	//LoadSection("Resources/StageJson/testNav.json", {0.0f, 0.0f, 0.0f}, SECTIONROTA::ROTA_0);
	LoadSection("Resources/StageJson/startNav.json", {0.0f, 0.0f, 0.0f}, SECTIONROTA::ROTA_0);
	LoadSection("Resources/StageJson/2Way_0.json", { -13.0f, 0.0f, 13.0f }, SECTIONROTA::ROTA_270);
	LoadSection("Resources/StageJson/4Way_0.json", { 0.0f, 0.0f, 13.0f }, SECTIONROTA::ROTA_0);
	LoadSection("Resources/StageJson/2Way_0.json", { 13.0f, 0.0f, 13.0f }, SECTIONROTA::ROTA_0);
	LoadSection("Resources/StageJson/3Way_0.json", { -13.0f, 0.0f, 26.0f }, SECTIONROTA::ROTA_270);
	LoadSection("Resources/StageJson/4Way_0.json", { 0.0f, 0.0f, 26.0f }, SECTIONROTA::ROTA_0);
	LoadSection("Resources/StageJson/3Way_0.json", { 13.0f, 0.0f, 26.0f }, SECTIONROTA::ROTA_90);
	LoadSection("Resources/StageJson/2Way_0.json", { -13.0f, 0.0f, 39.0f }, SECTIONROTA::ROTA_180);
	LoadSection("Resources/StageJson/3Way_0.json", { 0.0f, 0.0f, 39.0f }, SECTIONROTA::ROTA_180);
	LoadSection("Resources/StageJson/2Way_0.json", { 13.0f, 0.0f, 39.0f }, SECTIONROTA::ROTA_90);
	navMesh_->LinkCell();
	enemyMgr_->SetCellsCenter(navMesh_->GetCellsCenter());
}

void Stage::LoadSection(std::string fileName, const Vector3& offset, SECTIONROTA sectionRota)
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

	if (model_ == nullptr) model_ = std::make_unique<Model>("stoneBrick");

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		// MESH
		if (true)
		{
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});

			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			// モデル
			if (object.contains("file_name"))
			{
				// ファイル名
				objectData.fileName = object["file_name"];

				// 同じファイル名のモデルが無かったらモデル追加
				if (models_.count(object["file_name"]) == 0)
				{
					models_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
				}
			}

			// クラスの名前
			if (object.contains("class_name"))
			{
				objectData.className = object["class_name"];
			}

			// ｔランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			objectData.translation.x = -(float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			// 平行移動
			float sin = sinf(Util::Degree2Radian((float)sectionRota));
			float cos = cosf(Util::Degree2Radian((float)sectionRota));
			float oldX = objectData.translation.x;
			float oldY = objectData.translation.z;
			objectData.translation.x = cos * oldX - sin * oldY;
			objectData.translation.z = sin * oldX + cos * oldY;

			// スケーリング
			if (sectionRota == SECTIONROTA::ROTA_90 || sectionRota == SECTIONROTA::ROTA_270)
			{
				objectData.scale.x = (float)transform["scaling"][1];
				objectData.scale.y = (float)transform["scaling"][2];
				objectData.scale.z = (float)transform["scaling"][0];
			}

			else
			{
				objectData.scale.x = (float)transform["scaling"][0];
				objectData.scale.y = (float)transform["scaling"][2];
				objectData.scale.z = (float)transform["scaling"][1];
			}
		}
	}

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{

		if (objectData.className == "enemyCore")
		{
			enemyMgr_->AddCore(offset + objectData.translation);
		}

		else if (objectData.className == "Nav")
		{
			navMesh_->AddVertex(objectData.fileName, offset, (float)sectionRota);
		}

		else if (objectData.className == "collision")
		{
			// コライダー
			colliders_.emplace_front(std::make_unique<AABBCollider>());
			colliders_.front()->SetOffset(offset + objectData.translation);
			colliders_.front()->SetRadius(objectData.scale / 2.0f);
			colliders_.front()->SetAttribute(COL_STAGE_OBJ);
			colMgr_->AddBlockCollider(colliders_.front().get());
		}

		// オブジェクト追加
		else
		{
			// オブジェクト
			if (objects_->GetModel() == nullptr) objects_->SetModel(model_.get());
			objects_->AddTransform(offset + objectData.translation);
		}
	}
}