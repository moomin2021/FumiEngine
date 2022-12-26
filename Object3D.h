#pragma once
// Direct3D 12 用
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// 数学関数
#include <DirectXMath.h>
using namespace DirectX;

//　ComPtr用
#include <wrl.h>
using namespace Microsoft::WRL;

// シーケンスコンテナの一種
#include <vector>

// カメラクラス
#include "Camera.h"

// モデルクラス
#include "Model.h"

// 定数バッファ構造体(オブジェクト)
struct ObjectBuff {
	// 行列
	XMMATRIX mat;
};

// 定数バッファ構造体(マテリアル)
struct MaterialBuff {
	XMFLOAT3 ambient;// -> アンビエント係数
	float pad1;// -> パディング
	XMFLOAT3 diffuse;// -> ディフェーズ係数
	float pad2;// -> パディング
	XMFLOAT3 specular;// -> スペキュラー係数
	float alpha;// -> アルファ
};

class Object3D {
public:// メンバ変数
	XMFLOAT3 position_;// -> 座標
	XMFLOAT3 rotation_;// -> 回転角
	XMFLOAT3 scale_;// ----> スケール

private:// メンバ変数
	// 頂点データ関連
	D3D12_VERTEX_BUFFER_VIEW vbView_;// -----> 頂点バッファービュー
	ComPtr<ID3D12Resource> vertexBuff_;// ---> 頂点バッファ

	// インデックスデータ関連
	D3D12_INDEX_BUFFER_VIEW ibView_;// ---> インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;// -> インデックスバッファ

	// 定数バッファ
	ComPtr<ID3D12Resource> objectBuff_;// ---> オブジェクト
	ComPtr<ID3D12Resource> materialBuff_;// -> マテリアル

	// 透視投影行列
	XMMATRIX matProjection_;

	// カメラ
	Camera* camera_;

	// モデル
	Model* model_;

public:// メンバ関数
	// [Object3D]インスタンス作成
	Object3D* CreateObject3D();

	// 描画処理
	void Draw();

	// 描画前処理
	static void PreDraw();

#pragma region セッター
	// カメラを設定
	void SetCamera(Camera* camera) { camera_ = camera; }

	// モデルを設定
	void SetModel(Model* model);
#pragma endregion

private:// メンバ関数
	// コンストラクタ
	Object3D();

	// 初期化処理
	void Initialize();

	// マテリアルバッファ作成
	void CreateMaterialBuff();

	// 頂点バッファを作成
	void CreateVertexBuff();

	// インデックスバッファを作成
	void CreateIndexBuff();
};