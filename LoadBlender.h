#pragma once
#include "Object3D.h"
#include "Model.h"

#include <map>
#include <string>
#include <memory>

class LoadBlender
{
#pragma region 構造体
	// レベルデータ
	struct LevelData {
		// オブジェクト1個分のデータ
		struct ObjectData {
			// ファイル名
			std::string fileName;
			float3 translation;
			float3 rotation;
			float3 scaling;
		};

		std::vector<ObjectData> objects;
	};
#pragma endregion

#pragma region メンバ変数
private:
	// モデル
	std::map<std::string, std::unique_ptr<Model>> model_;

	// オブジェクト
	std::vector<std::unique_ptr<Object3D>> object_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoadBlender();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Jsonファイル読み込み
	/// </summary>
	void LoadJson(std::string fileName);
#pragma endregion
};