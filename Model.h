#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --シーケンスコンテナの一種-- //
#include <vector>

// --カメラクラス-- //
#include "Camera.h"

#include "Object3D.h"

#include <string>

// --定数バッファ用データ構造体（行列と色）-- //
struct ModelBufferData {
	// --行列-- //
	XMMATRIX mat;
};

// マテリアルバッファ用データ構造体
struct MaterialBufferData {
	XMFLOAT3 ambient;// -> アンビエント係数
	float pad1;// -> パディング
	XMFLOAT3 diffuse;// -> ディフェーズ係数
	float pad2;// -> パディング
	XMFLOAT3 specular;// -> スペキュラー係数
	float alpha;// -> アルファ
};

// マテリアル構造体
struct Material {
	std::string name;// -> マテリアル姪
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
	// --メンバ変数-- //
public:
	// 座標、回転角、スケール
	XMFLOAT3 position_;
	XMFLOAT3 rotation_;
	XMFLOAT3 scale_;

	// 色
	XMFLOAT4 color_;

private:
	// --頂点データ-- //
	std::vector<Vertices3D> vertices_;// -> 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vbView_;// -> 頂点バッファビュー
	ComPtr<ID3D12Resource> vertBuff_;// -> 頂点バッファ

	// --インデックスデータ-- //
	std::vector<uint16_t> indices_;// -> インデックスデータ
	D3D12_INDEX_BUFFER_VIEW ibView_;// -> インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;// -> インデックスバッファ

	// --モデル定数バッファ-- //
	ComPtr<ID3D12Resource> constBuff_;

	// マテリアル定数バッファ
	ComPtr<ID3D12Resource> materialBuff_;

	// --透視投影行列の計算-- //
	XMMATRIX matProjection_;

	// マテリアル
	Material material_;

	// テクスチャハンドル
	int textureHandle_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Model();

	// モデル読み込み
	void CreateModel(std::string name);

	// 更新処理
	void Update(Camera * camera);

	// 描画処理
	void Draw();

	// 描画前処理
	static void PreDraw();

private:
	// 頂点バッファとインデックスバッファを作成
	void CreateBuffer();

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);
};