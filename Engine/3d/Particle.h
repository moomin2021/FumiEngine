#pragma once
#include "Camera.h"
#include "Matrix4.h"
#include "float4.h"
#include "float2.h"

#include <wrl.h>
#include <d3d12.h>
#include <vector>

enum BILLBOARD {
	NONE,
	ALL,
	X,
	Y,
	Z
};

class Particle
{
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点データ
	struct Vertex {
		float3 pos;
	};

	// 定数バッファ用
	struct ConstBufferData {
		Matrix4 mat;
	};

#pragma region メンバ変数
private:
	// オブジェクトデータ
	float3 position_ = { 0.0f, 0.0f, 0.0f };
	float3 rotation_ = { 0.0f, 0.0f, 0.0f };
	float3 scale_ = { 1.0f, 1.0f, 1.0f };
	float4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 頂点データ
	std::vector<Vertex> vertices_;			// 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// 頂点バッファービュー
	ComPtr<ID3D12Resource> vertexBuff_;		// 頂点バッファ

	// インデックスデータ
	std::vector<uint16_t> indexes_;		// インデックスデータ
	D3D12_INDEX_BUFFER_VIEW indexView_;	// インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;	// インデックスバッファ

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	ConstBufferData* constMap_;

	// カメラ
	static Camera* sCamera_;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Particle();

	// 更新処理
	void Update(BILLBOARD billBoard = BILLBOARD::NONE);

	// 描画処理
	void Draw(uint16_t handle = 0);

private:
	// 頂点バッファ作成
	void CreateVertexBuff();

	// インデックスバッファ作成
	void CreateIndexBuff();
#pragma endregion

#pragma region セッター
public:
	// カメラを設定
	static void SetCamera(Camera* camera) { sCamera_ = camera; }
#pragma endregion
};