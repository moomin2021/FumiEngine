#pragma once

// 数学関数
#include <DirectXMath.h>
using namespace DirectX;

// シーケンスコンテナの一種
#include <vector>

// 文字列クラス
#include <string>

// 頂点データ
struct Vertex3D {
	XMFLOAT3 pos;// ----> 座標
	XMFLOAT3 normal;// -> 法線
	XMFLOAT2 uv;// -----> UV座標
};

// マテリアル構造体
struct Material {
	std::string name;// -> マテリアル名
	XMFLOAT3 ambient;// -> アンビエント影響度
	XMFLOAT3 diffuse;// -> ディフューズ影響度
	XMFLOAT3 specular;// -> スペキュラー影響度
	float alpha;// -> アルファ
	std::string textureFilename;// -> テクスチャファイル名

	// コンストラクタ
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}
};

class Model {
private:// メンバ変数
	std::vector<Vertex3D> vertexes_;// -> 頂点データ
	std::vector<uint16_t> indexes_;// --> インデックスデータ

	// マテリアル
	Material material_;

	// テクスチャハンドル
	int textureHandle_;

public:// メンバ関数
	// [Model]インスタンス作成
	Model* CreateModel(std::string fileName);

#pragma region ゲッター
	// 頂点データを取得
	std::vector<Vertex3D> GetVertexes() { return vertexes_; }

	// インデックスデータを取得
	std::vector<uint16_t> GetIndexes() { return indexes_; }

	// マテリアル取得
	Material GetMaterial() { return material_; }

	// テクスチャハンドルを取得
	int GetTextureHandle() { return textureHandle_; }
#pragma endregion

private:// メンバ関数
	// モデル読み込み
	void LoadModel(std::string name);

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);
};