#pragma once


#include <vector>
#include <d3d12.h>
#include <wrl.h>

class Model {
#pragma region エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

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
	uint16_t textureHandle_;
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