#pragma once
#include "StageObjectManager.h"
#include "Model.h"
#include "Vector3.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

class LoadStage {
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
	// インスタンス
	StageObjectManager* stageObjMgr_ = nullptr;

	// ファイル名と紐づけてモデルを保存
	std::map<std::string, std::unique_ptr<Model>> models_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	LoadStage();

	// デストラクタ
	~LoadStage();

	// ステージ読み込み
	void Load(std::string fileName);
#pragma endregion

#pragma region セッター関数
public:
	// ステージオブジェクト管理用クラスのインスタンスを取得
	void SetStageObjectManager(StageObjectManager* inst) { stageObjMgr_ = inst; }
#pragma endregion
};