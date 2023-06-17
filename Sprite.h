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

class Sprite {
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
	float2 position_;	// 座標
	float rotation_;	// 回転
	float2 scale_;		// 拡縮
	float4 color_;		// 色(RGBA)

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
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(int textureHandle = 0);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

private:
	/// <summary>
	/// スプライトデータの更新
	/// </summary>
	void UpdateData();

#pragma region セッター関数
public:

	/// <summary>
	/// 座標(XY)を設定
	/// </summary>
	/// <param name="position"> 座標(XY) </param>
	inline void SetPosition(const float2& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// 回転(Z)を設定
	/// </summary>
	/// <param name="rotation"> 回転(Z) </param>
	inline void SetRotation(float rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// 拡縮(XY)を設定
	/// </summary>
	/// <param name="scale"> 拡縮(XY) </param>
	inline void SetScale(const float2& scale) { scale_ = scale, hasChanget_ = true; }

	/// <summary>
	/// 色(RGBA)を設定
	/// </summary>
	/// <param name="color"> 色(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }
#pragma endregion

#pragma endregion
};