#pragma once
#include "float3.h"
#include "float2.h"

#include <string>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12.lib")

class Model {
	// 頂点データ
	struct Vertex {
		float3 pos;		// 座標(XYZ)
		float3 normal;	// 法線(XYZ)
		float2 uv;		// UV座標(XY)
	};

	// マテリアルデータ
	struct Material {
		std::string name;	// マテリアル名
		float3 ambient;		// アンビエント影響度
		float3 diffuse;		// ディフューズ影響度
		float3 specular;	// スペキュラー影響度
		float alpha;		// アルファ

		// テクスチャファイル名
		std::string textureFilename;

		// コンストラクタ
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	// 定数バッファ用マテリアルデータ
	struct MaterialBuffer {
		float3 ambient;	// アンビエント係数
		float pad1;		// パディング
		float3 diffuse;	// ディフェーズ係数
		float pad2;		// パディング
		float3 specular;// スペキュラー係数
		float alpha;	// アルファ
	};

#pragma region メンバ変数
private:
	// 頂点データ
	std::vector<Vertex> vertex_;			// 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// 頂点バッファービュー
	ComPtr<ID3D12Resource> vertexBuff_;		// 頂点バッファ

	// インデックスデータ
	std::vector<uint16_t> index_;		// インデックスデータ
	D3D12_INDEX_BUFFER_VIEW indexView_;	// インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;	// インデックスバッファ

	// マテリアルデータ
	Material material_;						// マテリアルデータ
	ComPtr<ID3D12Resource> materialBuff_;	// マテリアルバッファ

	// テクスチャハンドル
	int textureHandle_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName"> モデルファイル名 </param>
	Model(std::string fileName);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="name"> ファイル名 </param>
	void LoadModel(std::string name);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"> ファイル名 </param>
	/// <param name="fileName"> ファイル名 </param>
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// 頂点バッファ作成
	/// </summary>
	void CreateVertexBuff();

	/// <summary>
	/// インデックスバッファ作成
	/// </summary>
	void CreateIndexBuff();

	/// <summary>
	/// マテリアルバッファ作成
	/// </summary>
	void CreateMaterialBuff();
#pragma endregion
};