#pragma once
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

class PostEffect {
	// 頂点データ
	struct Vertex {
		float2 pos;	// 座標(XYZ)
		float2 uv;	// UV座標(XY)
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Matrix4 mat;	// 行列
		float4 color;	// 色(RGBA)
	};

#pragma region メンバ変数
private:
	// スプライトデータ
	float2 position_;	// 座標(XY)
	float rotation_;	// 回転(Z)
	float2 size_;		// サイズ(XY)
	float4 color_;		// 色(RGBA)
	float2 anchorPoint_;// アンカーポイント座標(XY)
	bool isFlipX_;		// 左右反転
	bool isFlipY_;		// 上下反転

	// スプライトデータを変更したかどうか
	bool hasChanget_;

	// 行列
	Matrix4 matWorld_;		// ワールド行列
	Matrix4 matProjection_;	// 射影変換行列

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;	// 定数バッファ
	ConstBufferData* constMap_;			// マッピング処理用

	// 頂点データ
	std::vector<Vertex> vertex_;		// 頂点データ
	ComPtr<ID3D12Resource> vertexBuff_;		// 頂点バッファ
	Vertex* vertexMap_;						// マッピング処理用
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// 頂点バッファビュー

	// インデックスデータ
	std::vector<uint16_t> index_;	// インデックスデータ
	ComPtr<ID3D12Resource> indexBuff_;	// インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW indexView_;	// インデックスバッファビュー

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff_;

	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;

	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	// 画面クリアカラー
	static const std::vector<float> clearColor_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private:
	/// <summary>
	/// スプライトデータの更新
	/// </summary>
	void UpdateData();

	/// <summary>
	/// 頂点バッファ作成
	/// </summary>
	void CreateVertexBuff();

	/// <summary>
	/// インデックスバッファ作成
	/// </summary>
	void CreateIndexBuff();

	/// <summary>
	/// 定数バッファ作成
	/// </summary>
	void CreateConstBuff();
#pragma endregion
};