#pragma once
#include "Model.h"
#include "Object3D.h"
#include "AABBCollider.h"
#include "CollisionManager.h"
#include "Instancing3D.h"

#include "EnemyManager.h"
#include "NavMesh.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <forward_list>

class Stage
{
	// レベルデータ
	struct LevelData {
		// オブジェクト1個分のデータ
		struct ObjectData {
			// ファイル名
			std::string fileName;
			std::string className;
			Vector3 translation;
			Vector3 scale;
		};

		std::vector<ObjectData> objects;
	};

	enum class SECTIONROTA {
		ROTA_0 = 0,
		ROTA_90 = 90,
		ROTA_180 = 180,
		ROTA_270 = 270,
	};

	enum class SECTION : uint16_t {
		WAY2 = 2,
		WAY3 = 3,
		WAY4 = 4,
	};

#pragma region メンバ変数
private:
	// 衝突判定管理クラスインスタンス
	CollisionManager* colMgr_ = nullptr;
	EnemyManager* enemyMgr_ = nullptr;

	// モデル保存用連想配列
	std::unordered_map<std::string, std::unique_ptr<Model>> models_ = {};

	// オブジェクト
	std::unique_ptr<Instancing3D> objects_ = nullptr;
	std::unique_ptr<Model> model_ = nullptr;

	// コライダー
	std::forward_list<std::unique_ptr<AABBCollider>> colliders_ = {};

	// スカイドーム
	std::unique_ptr<Model> mSkydome_	= nullptr;// モデル
	std::unique_ptr<Object3D> oSkydome_	= nullptr;// オブジェクト

	// ナビメッシュ
	std::unique_ptr<NavMesh> navMesh_ = nullptr;

	// 各区画のバリエーション数
	const uint16_t max2Way_ = 1;
	const uint16_t max3Way_ = 1;
	const uint16_t max4Way_ = 2;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 初期化処理
	void Initialize();

	// 行列更新処理
	void MatUpdate();

	// 描画更新処理
	void Draw();

	// ステージ情報読み込み
	void Load(std::string fileName, bool isCol = true, bool isCore = false);

	void CreateStage();

	void LoadSection(std::string fileName, const Vector3& offset, SECTIONROTA sectionRota);

	std::string RandomSection(SECTION selectSection);
#pragma endregion

#pragma region セッター関数
	void SetEnemyManager(EnemyManager* inst) { enemyMgr_ = inst; }
#pragma endregion
};
