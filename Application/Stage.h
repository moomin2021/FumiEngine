#pragma once
#include "Model.h"
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

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
			int isActive;
			std::string fileName;
			std::string className;
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;
		};

		std::vector<ObjectData> objects;
	};

#pragma region メンバ変数
private:
	// 衝突判定管理クラスインスタンス
	CollisionManager* colMgr_ = nullptr;

	// モデル保存用連想配列
	std::unordered_map<std::string, std::unique_ptr<Model>> models_ = {};

	// オブジェクト
	std::forward_list<std::unique_ptr<Object3D>> objects_ = {};

	// コライダー
	std::forward_list<std::unique_ptr<MeshCollider>> colliders_ = {};
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
	void Load(std::string fileName);
#pragma endregion
};